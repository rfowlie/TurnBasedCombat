// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatWorldSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TurnBased_Core_Tags.h"
#include "AI/ActionEvaluator_Combat.h"
#include "Combat/CombatCalculator_Basic.h"
#include "Combat/CombatEventWrapper.h"
#include "GameMode/GameMode_TurnBased_Combat.h"
#include "Grid/GridHelper.h"
#include "Grid/GridWorldSubsystem.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"
#include "Unit/Components/GridUnitBehaviourComponent.h"



void UCombatWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();

	CombatEvaluator = NewObject<UActionEvaluator_Combat>();

	// bind to turn change so we can tell when enemy factions turn...
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	TurnWorldSubsystem->OnFactionStart.AddUniqueDynamic(this, &ThisClass::ExecuteEnemyTurn);
}

void UCombatWorldSubsystem::InitiateCombat(FCombatPrediction InCombatPrediction)
{
	if (InCombatPrediction.CombatOrder.IsEmpty()) { return; }

	CombatPrediction = InCombatPrediction;

	// TODO: for now... just move units here...
	CombatPrediction.CombatInformation.InstigatorUnit->SetActorLocation(CombatPrediction.CombatInformation.InstigatorTile->GetPlacementLocation());
	
	if (OnCombatStart.IsBound()) { OnCombatStart.Broadcast(
		InCombatPrediction.CombatInformation.InstigatorUnit,
		InCombatPrediction.CombatInformation.TargetUnit
		); }
	
	DoCombatTurn();
}

void UCombatWorldSubsystem::DoCombatTurn()
{
	ActiveUnit = CombatPrediction.CombatOrder[0];
	CombatPrediction.CombatOrder.RemoveAt(0);

	DelegateHandle = ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);

	// send gameplay event
	FGameplayEventData EventData;
	EventData.Instigator = ActiveUnit;
	EventData.Target = ActiveUnit == CombatPrediction.CombatInformation.InstigatorUnit ?
		CombatPrediction.CombatInformation.TargetUnit : CombatPrediction.CombatInformation.InstigatorUnit;
	
	UCombatEventWrapper* EventWrapper = NewObject<UCombatEventWrapper>();
	EventWrapper->InstigatorSnapShotAdvanced = ActiveUnit == CombatPrediction.CombatInformation.InstigatorUnit ?
		CombatPrediction.TargetSnapshotAdvanced : CombatPrediction.InstigatorSnapShotAdvanced;
	EventData.OptionalObject = EventWrapper;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, TAG_Event_Grid_Attack, EventData);
}

void UCombatWorldSubsystem::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
{
	// remove delegate otherwise odd behaviour
	if (InGameplayAbility->GetAvatarActorFromActorInfo() == ActiveUnit)
	{
		ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.Remove(DelegateHandle);
		
		if (!CombatPrediction.CombatOrder.IsEmpty() &&
		 CombatPrediction.CombatInformation.InstigatorUnit->GetHealth() > 0 && CombatPrediction.CombatInformation.TargetUnit->GetHealth() > 0)
		{
			DoCombatTurn();
		}
		else
		{
			// zero out values???
			// ActiveUnit = nullptr;
			// CombatPrediction = FCombatPrediction();
			
			// should call this every time a combat finishes, not just when all units finished (AI)
			if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(CombatPrediction); }
			
			// TODO: a bit odd, we want the turn manager to still function properly
			// ALREADY HAPPENING TURN MANAGER HAS CALLBACK
			// Lets try manually doing this for better control???
			UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
			TurnWorldSubsystem->SetUnitTurnOver(CombatPrediction.CombatInformation.InstigatorUnit);

			if (!UnitsToExecuteTurns.IsEmpty())
			{
				InitiateUnitCombat(FCombatPrediction());
				// // just add some delay so it's not so crazy fast
				// FTimerHandle TimerHandle;
				// FTimerDelegate TimerDelegate;
				// TimerDelegate.BindLambda([this]()
				// {
				// 	InitiateUnitCombat(FCombatPrediction());
				// });
				//
				// GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.0f, false, 2.0f);
			}
		}
	}
}

void UCombatWorldSubsystem::ExecuteEnemyTurn(FGameplayTag FactionTag)
{
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (FactionTag == TurnWorldSubsystem->GetPlayerFactionTag()) { return; }
	
	UnitsToExecuteTurns.Empty();
	UnitsToExecuteTurns = TurnWorldSubsystem->GetUnitsInFaction(FactionTag);
	// OnCombatEnd.AddUniqueDynamic(this, &ThisClass::InitiateUnitCombat);
	// TODO: THIS IS HACKYYYYYYYYY... need a better way than to pass this...
	InitiateUnitCombat(FCombatPrediction());
}

void UCombatWorldSubsystem::InitiateUnitCombat(const FCombatPrediction& InCombatPrediction)
{
	if (UnitsToExecuteTurns.IsEmpty())
	{
		// TODO: what goes here...
		return;
	}

	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	UGridWorldSubsystem* GridWorldSubsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	
	ActiveUnit = UnitsToExecuteTurns.Pop();
	TArray<FCombatPrediction> CombatPredictions;
	TMap<AGridUnit*, FGridTileArray> PossibleAttacksMap = GridWorldSubsystem->GetEnemiesInRangeWithAttackTiles(ActiveUnit);

	// get all possible combats (predictions)
	for (auto GridUnitTilePair : PossibleAttacksMap)
	{
		AGridTile* TargetsGridTile = GridWorldSubsystem->GetGridTileOfUnit(GridUnitTilePair.Key);
		FName TargetWeapon = GridUnitTilePair.Key->GetEquippedWeaponName();
		for (auto GridTile : GridUnitTilePair.Value.GridTiles)
		{
			for (FName Weapon : ActiveUnit->GetWeaponsInMap())
			{
				FCombatPrediction OutCombatPrediction;
				FCombatInformation CombatInformation;
				CombatInformation.InstigatorUnit = ActiveUnit;
				CombatInformation.InstigatorTile = GridTile;
				CombatInformation.InstigatorWeapon = Weapon;
				CombatInformation.TargetUnit = GridUnitTilePair.Key;
				CombatInformation.TargetTile = TargetsGridTile;
				CombatInformation.TargetWeapon = TargetWeapon;
				GameMode->GetCombatCalculator()->GetCombatPrediction(OutCombatPrediction, CombatInformation);

				// don't allow it as an option if neither unit can attack or instigator cannot reach with weapon range
				if (!OutCombatPrediction.CombatOrder.IsEmpty() && OutCombatPrediction.CombatOrder.Contains(OutCombatPrediction.CombatInformation.InstigatorUnit))
				{
					CombatPredictions.Add(OutCombatPrediction);
				}				
			}
		}
	}

	if (!CombatPredictions.IsEmpty())
	{
		// determine best action to take
		for (FCombatPrediction& Prediction : CombatPredictions)
		{
			FCombatEvaluation ObjectiveCombatEvaluation;
			CombatEvaluator->GetCombatEvaluation(ObjectiveCombatEvaluation, Prediction);
			FCombatEvaluation UnitCombatEvaluation;
			Prediction.CombatInformation.InstigatorUnit->CombatBehaviourComponent->GetCombatEvaluation(UnitCombatEvaluation, Prediction);

			// assign evaluation to prediction
			Prediction.CombatEvaluation = ObjectiveCombatEvaluation * UnitCombatEvaluation;
		}

		// CombatPredictions.Sort();
		Algo::Sort(CombatPredictions);
		InitiateCombat(CombatPredictions[0]);
	}
	// no combat predictions... skip turn???
	else
	{
		// TODO: hacky for the same above reasons...
		InitiateUnitCombat(FCombatPrediction());
	}	
}

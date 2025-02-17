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
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/PlayerController_TurnBased.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"
#include "Unit/Components/GridUnitBehaviourComponent.h"


DEFINE_LOG_CATEGORY(Log_Combat);

void UCombatWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();

	CombatEvaluator = NewObject<UActionEvaluator_Combat>();

	// bind to turn change so we can tell when enemy factions turn...
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	TurnWorldSubsystem->OnFactionStart.AddUniqueDynamic(this, &ThisClass::StartTurnAI);
}

void UCombatWorldSubsystem::InitiateCombat(FCombatPrediction InCombatPrediction)
{
	if (InCombatPrediction.CombatOrder.IsEmpty()) { return; }

	CombatPrediction = InCombatPrediction;

	// TODO: for now... just move units here...
	CombatPrediction.CombatInformation.InstigatorUnit->SetActorLocation(CombatPrediction.CombatInformation.InstigatorTile->GetPlacementLocation());
	
	CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	CombatPrediction.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	
	if (OnCombatStart.IsBound()) { OnCombatStart.Broadcast(
		InCombatPrediction.CombatInformation.InstigatorUnit,
		InCombatPrediction.CombatInformation.TargetUnit
		); }
	
	SendCombatEventToNextUnit();
}

void UCombatWorldSubsystem::SendCombatEventToNextUnit()
{
	AGridUnit* ActiveUnit = CombatPrediction.CombatOrder[0];
	// CombatPrediction.CombatOrder.RemoveAt(0);

	// DelegateHandle = ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);

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
	if (InGameplayAbility->GetAvatarActorFromActorInfo() == CombatPrediction.CombatOrder[0])
	{
		float GameTime = GetWorld()->GetTimeSeconds();
		UE_LOG(Log_Combat, Log, TEXT("Game Time: %f"), GameTime);
		UE_LOG(Log_Combat, Log, TEXT("OnCombatComplete - %s, %s"), *CombatPrediction.CombatOrder[0]->GetName(), *CombatPrediction.Id.ToString());
		// CombatPrediction.CombatOrder[0]->GetAbilitySystemComponent()->AbilityEndedCallbacks.Remove(DelegateHandle);
		CombatPrediction.CombatOrder.RemoveAt(0);

		if (!CombatPrediction.CombatOrder.IsEmpty() &&
			CombatPrediction.CombatInformation.InstigatorUnit->GetHealth() > 0 &&
			CombatPrediction.CombatInformation.TargetUnit->GetHealth() > 0)
		{
			SendCombatEventToNextUnit();
		}
		else
		{
			CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
			CombatPrediction.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);

			AGridUnit* InstigatorUnit = CombatPrediction.CombatInformation.InstigatorUnit;
			
			// should call this every time a combat finishes, not just when all units finished (AI)
			if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(CombatPrediction); }

			// zero out...
			CombatPrediction = FCombatPrediction();
			
			// Update unit that attacked
			UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
			TurnWorldSubsystem->SetUnitTurnOver(InstigatorUnit);
			TurnWorldSubsystem->CheckFactionTurnComplete(InstigatorUnit->Execute_GetFaction(InstigatorUnit));
		}
	}
}

void UCombatWorldSubsystem::OnCombatCompleteAI(UGameplayAbility* InGameplayAbility)
{
	// remove delegate otherwise odd behaviour
	if (InGameplayAbility->GetAvatarActorFromActorInfo() == CombatPredictionAI.CombatOrder[0])
	{
		float GameTime = GetWorld()->GetTimeSeconds();
		UE_LOG(Log_Combat, Log, TEXT("Game Time AI: %f"), GameTime);
		UE_LOG(Log_Combat, Log, TEXT("OnCombatCompleteAI - %s, %s"), *CombatPredictionAI.CombatOrder[0]->GetName(), *CombatPredictionAI.Id.ToString());
		// CombatPredictionAI.CombatOrder[0]->GetAbilitySystemComponent()->AbilityEndedCallbacks.Remove(DelegateHandleAI);
		CombatPredictionAI.CombatOrder.RemoveAt(0);
		
		if (!CombatPredictionAI.CombatOrder.IsEmpty() &&
			CombatPredictionAI.CombatInformation.InstigatorUnit->GetHealth() > 0 &&
			CombatPredictionAI.CombatInformation.TargetUnit->GetHealth() > 0)
		{
			SendCombatEventAI();
		}
		else
		{
			CombatPredictionAI.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
			CombatPredictionAI.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
			
			// should call this every time a combat finishes, not just when all units finished (AI)
			if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(CombatPredictionAI); }

			// TODO: seems a bit hacky right now...
			// UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
			// TurnWorldSubsystem->SetUnitTurnOver(CombatPredictionAI.CombatInformation.InstigatorUnit);

			if (!AIUnitsToExecuteTurns.IsEmpty())
			{
				GetNextCombatPredictionAI();
			}
			else
			{
				UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
				TurnWorldSubsystem->SetFactionTurnComplete(ActiveFactionAI);
			}
		}
	}
}

void UCombatWorldSubsystem::SendCombatEventAI()
{
	// send gameplay event to instigator
	AGridUnit* ActiveUnit = CombatPredictionAI.CombatOrder[0];
	// CombatPrediction.CombatOrder.RemoveAt(0);

	// DelegateHandleAI = ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnCombatCompleteAI);

	// send gameplay event
	FGameplayEventData EventData;
	EventData.Instigator = ActiveUnit;
	EventData.Target = ActiveUnit == CombatPredictionAI.CombatInformation.InstigatorUnit ?
		CombatPredictionAI.CombatInformation.TargetUnit : CombatPredictionAI.CombatInformation.InstigatorUnit;
	
	UCombatEventWrapper* EventWrapper = NewObject<UCombatEventWrapper>();
	EventWrapper->InstigatorSnapShotAdvanced = ActiveUnit == CombatPredictionAI.CombatInformation.InstigatorUnit ?
		CombatPredictionAI.TargetSnapshotAdvanced : CombatPredictionAI.InstigatorSnapShotAdvanced;
	EventData.OptionalObject = EventWrapper;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, TAG_Event_Grid_Attack, EventData);
}

// moving units to tiles and broadcasting events
void UCombatWorldSubsystem::SetupCombatAI()
{
	if (CombatPredictionAI.CombatOrder.IsEmpty()) { return; }

	// make camera follow
	if (APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Pawn->SetFollowTarget(CombatPredictionAI.CombatInformation.InstigatorUnit);
	}
	
	// TODO: for now... just move units here...
	CombatPredictionAI.CombatInformation.InstigatorUnit->SetActorLocation(CombatPredictionAI.CombatInformation.InstigatorTile->GetPlacementLocation());

	CombatPredictionAI.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnCombatCompleteAI);
	CombatPredictionAI.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnCombatCompleteAI);
	
	// broadcast combat start
	if (OnCombatStart.IsBound()) { OnCombatStart.Broadcast(
		CombatPredictionAI.CombatInformation.InstigatorUnit,
		CombatPredictionAI.CombatInformation.TargetUnit
		); }

	SendCombatEventAI();
}

void UCombatWorldSubsystem::GetNextCombatPredictionAI()
{
	if (AIUnitsToExecuteTurns.IsEmpty())
	{
		// TODO: what goes here...
		return;
	}

	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	UGridWorldSubsystem* GridWorldSubsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>();

	AGridUnit* ActiveUnit = AIUnitsToExecuteTurns[0];
	AIUnitsToExecuteTurns.RemoveAt(0);
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
		CombatPredictionAI = CombatPredictions[0];
		SetupCombatAI();
	}
	// no combat predictions... skip turn???
	else
	{
		// end unit turn if no attack options available otherwise will sit...
		UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
		TurnWorldSubsystem->SetUnitTurnOver(ActiveUnit);
		
		// TODO: hacky for the same above reasons...
		GetNextCombatPredictionAI();
	}	
}

void UCombatWorldSubsystem::StartTurnAI(FGameplayTag FactionTag)
{
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (FactionTag == TurnWorldSubsystem->GetPlayerFactionTag()) { return; }

	ActiveFactionAI = FactionTag;
	AIUnitsToExecuteTurns.Empty();
	AIUnitsToExecuteTurns = TurnWorldSubsystem->GetAliveUnitsInFaction(FactionTag);
	
	// TODO: THIS IS HACKYYYYYYYYY... need a better way than to pass this...
	GetNextCombatPredictionAI();
}

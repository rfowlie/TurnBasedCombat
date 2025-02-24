// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIWorldSubsystem.h"

#include "AI/ActionEvaluator_Combat.h"
#include "Combat/CombatCalculator_Basic.h"
#include "Combat/CombatWorldSubsystem.h"
#include "GameMode/GameMode_TurnBased_Combat.h"
#include "Grid/GridWorldSubsystem.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/Components/GridUnitBehaviourComponent.h"



void UAIWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
	
	CombatEvaluator = NewObject<UActionEvaluator_Combat>();
	
	// bind to turn change so we can tell when enemy factions turn...
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	TurnWorldSubsystem->OnFactionStart.AddUniqueDynamic(this, &ThisClass::StartTurn);
}

void UAIWorldSubsystem::StartTurn(FGameplayTag FactionTag)
{
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (FactionTag == TurnWorldSubsystem->GetPlayerFactionTag()) { return; }
	
	if (bTurnActive)
	{
		UE_LOG(Log_Combat, Error, TEXT("AI turn already active - %s, trying to start AI turn - %s"),
			*ActiveFaction.ToString(), *FactionTag.ToString());
		return;
	}

	bTurnActive = true;
	ActiveFaction = FactionTag;
	UnitsToExecuteTurnsIndex = 0;
	UnitsToExecuteTurns.Empty();
	UnitsToExecuteTurns = TurnWorldSubsystem->GetAliveUnitsInFaction(ActiveFaction);
	GetCombatPrediction();
}

void UAIWorldSubsystem::EndTurn(FGameplayTag FactionTag)
{
	UCombatWorldSubsystem* CombatWorldSubsystem = GetWorld()->GetSubsystem<UCombatWorldSubsystem>();
	CombatWorldSubsystem->OnCombatEnd.RemoveAll(this);
	
	// end unit turn if no attack options available otherwise will sit...
	UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	for (auto GridUnit : UnitsToExecuteTurns)
	{
		TurnWorldSubsystem->SetUnitTurnOver(GridUnit);
	}
	
	TurnWorldSubsystem->CheckFactionTurnComplete(ActiveFaction);

	ActiveFaction = FGameplayTag();
	CombatPrediction = FCombatPrediction();
	bTurnActive = false;
}

void UAIWorldSubsystem::GetCombatPrediction()
{
	if (!UnitsToExecuteTurns.IsValidIndex(UnitsToExecuteTurnsIndex))
	{
		EndTurn(ActiveFaction);
		return;
	}
	
	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	UGridWorldSubsystem* GridWorldSubsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	
	AGridUnit* ActiveUnit = UnitsToExecuteTurns[UnitsToExecuteTurnsIndex];
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
		CombatPrediction = CombatPredictions[0];
		UCombatWorldSubsystem* CombatWorldSubsystem = GetWorld()->GetSubsystem<UCombatWorldSubsystem>();
		CombatWorldSubsystem->OnCombatEnd.AddUniqueDynamic(this, &ThisClass::OnCombatEnd);
		CombatWorldSubsystem->InitiateCombat(CombatPrediction);
	}
	// no combat predictions... skip turn???
	else
	{
		CombatPrediction = FCombatPrediction();
		UnitsToExecuteTurnsIndex++;
		GetCombatPrediction();
	}
}

void UAIWorldSubsystem::OnCombatEnd(const FCombatPrediction& InCombatPrediction)
{
	if (bTurnActive && CombatPrediction.Id == InCombatPrediction.Id)
	{
		CombatPrediction = FCombatPrediction();
		UnitsToExecuteTurnsIndex++;
		GetCombatPrediction();
	}
	else
	{
		UE_LOG(Log_Combat, Error, TEXT("AI On Combat End Firing but turn NOT active: %s"),
			*InCombatPrediction.GetString());
	}
}

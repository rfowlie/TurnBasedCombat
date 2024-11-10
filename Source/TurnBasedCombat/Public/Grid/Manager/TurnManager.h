// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/Unit/GridUnit.h"
#include "UObject/Object.h"
#include "TurnManager.generated.h"


class AGridUnit;


USTRUCT()
struct FFactionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;
	
	UPROPERTY()
	TMap<AGridUnit*, bool> GridUnits;

	void ActivateUnits()
	{
		TArray<AGridUnit*> Units;
		GridUnits.GetKeys(Units);
		for (AGridUnit* GridUnit : Units)
		{
			if (GridUnit->GetHealth() > 0)
			{
				GridUnits[GridUnit] = true;
			}
		}
	}
	
	bool IsFactionDefeated() const
	{
		TArray<AGridUnit*> Units;
		GridUnits.GetKeys(Units);
		for (AGridUnit* GridUnit : Units)
		{
			if (GridUnit->GetHealth() > 0)
			{
				return false;
			}
		}

		return true;
	}

	bool CanUnitsMove()
	{
		for (auto Pair : GridUnits)
		{
			if (Pair.Value) { return true; }
		}

		return false;
	}
};


/**
 * FOR NOW
 * this is just a simple turn manager that determines a turn simply with a bool
 * in the future we can create a turn manager interface and create alternate turn managers
 * to accomodate different types of games styles
 */
UCLASS()
class TURNBASEDCOMBAT_API UTurnManager : public UObject
{
	GENERATED_BODY()

public:
	UTurnManager();

	// grid manager will set a function that it wants this to call when someone wins
	DECLARE_DELEGATE_OneParam(FFactionEventDelegate, FName)
	FFactionEventDelegate OnFactionDefeated;	
	
	DECLARE_EVENT_OneParam(UTurnManager, FTurnEvent, FName)
	FTurnEvent OnTurnBegin;
	FTurnEvent OnTurnFinish;
	
	void RegisterGridUnit(AGridUnit* GridUnit);
	void UnregisterGridUnit(AGridUnit* GridUnit);

	FName GetCurrentFaction() const;	
	bool CanTakeTurn(AGridUnit* GridUnit);
	void UpdateGridUnitActionTaken(AGridUnit* GridUnit);

	void SetNextFaction();
	void GetActiveFactions(TArray<FName>& ActiveFactions);
	
	UFUNCTION()
	void CheckFactionDefeated(AGridUnit* GridUnit);
	
protected:
	UPROPERTY()
	int32 FactionIndex = -1;
	void IncrementFaction();
	UPROPERTY()
	TArray<FFactionInfo> Factions;

};

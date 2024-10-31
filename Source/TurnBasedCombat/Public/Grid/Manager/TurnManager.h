// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TurnManager.generated.h"


class AGridUnit;

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
	DECLARE_DELEGATE_OneParam(FFactionWin, FName)
	FFactionWin OnFactionWin;
	
	DECLARE_EVENT_OneParam(UTurnManager, FTurnEvent, FName)
	FTurnEvent OnTurnBegin;
	FTurnEvent OnTurnFinish;
	
	void RegisterGridUnit(AGridUnit* GridUnit);
	void UnregisterGridUnit(AGridUnit* GridUnit);

	FName GetCurrentFaction() const;	
	bool CanTakeTurn(AGridUnit* GridUnit);
	void UpdateGridUnitMoved(AGridUnit* GridUnit);
	void UpdateGridUnitAttacked(AGridUnit* GridUnit);
	void UpdateGridUnitKilled(AGridUnit* GridUnit);

	void SetNextFaction();
	
private:
	UPROPERTY()
	TMap<AGridUnit*, bool> GridUnitsMap;
	UPROPERTY()
	TArray<FName> Factions;
	UPROPERTY()
	TMap<FName, bool> FactionsMap;
	UPROPERTY()
	int32 FactionIndex = -1;

	void IncrementFaction();	
	bool CheckGridUnit(const AGridUnit* GridUnit) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TurnManager.generated.h"


class AGridUnit;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UTurnManager : public UObject
{
	GENERATED_BODY()

public:
	UTurnManager();
	
	DECLARE_EVENT_OneParam(UTurnManager, FTurnEvent, FName)
	FTurnEvent OnTurnBegin;
	FTurnEvent OnTurnFinish;
	
	void RegisterGridUnit(AGridUnit* GridUnit);
	void UnregisterGridUnit(AGridUnit* GridUnit);
	
	bool CanTakeTurn(AGridUnit* GridUnit);

private:
	TMap<AGridUnit*, bool> GridUnitsMap;
	TArray<FName> Factions;
	int32 FactionIndex = 0;

	// TODO: bind this to GameModeNextTurn???
	void SetNextFaction();
};

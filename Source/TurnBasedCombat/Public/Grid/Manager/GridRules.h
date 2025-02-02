// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridRules.generated.h"


class UGridManager;
class AGridTile;
class AGridUnit;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UGridRules : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool UnitCanMoveOnTile(UGridManager* GridManager, AGridUnit* InUnit, AGridTile* StartTile, AGridTile* TargetTile);	

	UFUNCTION()
	int32 CalculateTerrainCost(UGridManager* GridManager, AGridUnit* InUnit, AGridTile* StartTile, AGridTile* TargetTile);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridStructs.h"
#include "UObject/Object.h"
#include "GridMap_FlatNoHeight.generated.h"

class AGridUnit;

/**
 * logic for a basic 2D grid with no Z axis
 * (will impact logic and demands from units and tiles)
 */
UCLASS()
class TURNBASEDCOMBAT_API UGridMap_FlatNoHeight : public UObject
{
	GENERATED_BODY()

public:
	inline static TArray<FGridPosition> DirectionsStraight = {
		FGridPosition(-1, 0),
		FGridPosition(1, 0),
		FGridPosition(0, -1),
		FGridPosition(0, 1)
	};

	inline static TArray<FGridPosition> DirectionsDiagonal = {
		FGridPosition(-1, -1),
		FGridPosition(-1, 1),
		FGridPosition(1, -1),
		FGridPosition(1, 1)
	};		
	
	// UFUNCTION(BlueprintCallable)
	// static int32 GetGridSize() { return 200; }
	
	// UFUNCTION(BlueprintCallable)
	// static FGridPosition CalculateGridPosition(const AGridInfo* GridInfo, const AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	static FGridPosition CalculateGridPosition(const AActor* Actor);
	
	// OTHER
	void CalculateGridAttackTiles(TMap<AGridTile*, int32>& OutWeaponPositions,
	const TArray<FGridMovement>& InGridMovements,
	const AGridUnit* InstigatorUnit,
	const TArray<int32>& WeaponRanges,
	const AGridUnit* TargetUnit);
};

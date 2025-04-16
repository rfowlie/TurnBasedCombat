// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridHelper.generated.h"

class AATBGridTile;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UGridHelper : public UBlueprintFunctionLibrary
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

	UFUNCTION(BlueprintCallable)
	static int32 GetDistanceBetweenGridPositions(const FGridPosition& positionA, const FGridPosition& positionB);

	UFUNCTION(BlueprintCallable)
	static int32 GetGridSize() { return 200; }

	UFUNCTION(BlueprintCallable)
	static FGridPosition CalculateGridPosition(const AActor* Actor);
	
	// returns all grid coordinates that are at the given range
	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsAtRange(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& OutGridPositions);

	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations);

	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsInRange(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);
};

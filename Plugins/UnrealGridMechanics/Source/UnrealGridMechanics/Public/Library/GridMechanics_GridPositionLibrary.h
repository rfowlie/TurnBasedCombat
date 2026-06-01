// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridMechanics_Structs.h"
#include "GridMechanics_GridPositionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGRIDMECHANICS_API UGridMechanics_GridPositionLibrary : public UBlueprintFunctionLibrary
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

	// size will be variable depending on the game
	UFUNCTION(BlueprintCallable)
	static FGridPosition CalculateGridPositionFromSize(const AActor* Actor, int32 Size);

	// returns all grid coordinates that are at the given range
	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsAtRange(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);

	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations);

	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsInRange(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);
	
};

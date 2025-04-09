// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridSystemsStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridSystemsUtility.generated.h"

class AGridInfo;
class AATBGridTile;

/**
 * 
 */
UCLASS()
class TURNBASEDGRIDSYSTEMS_API UGridSystemsUtility : public UBlueprintFunctionLibrary
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

	UFUNCTION(BlueprintCallable)
	static FGridPosition CalculateGridPosition(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	static FGridPosition CalculateGridPositionWithInfo(const AGridInfo* GridInfo, const AActor* Actor);

	// Grid Measurements
	// This measures distance by only allowing movement in horizontal and vertical steps, form a diamond-shaped region
	UFUNCTION(BlueprintCallable)
	static void GetManhattanDistance(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);

	UFUNCTION(BlueprintCallable)
	static void GetManhattanDistanceAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations);

	// forms a square, chess king's movement where diagonal costs the same...
	UFUNCTION(BlueprintCallable)
	static void GetChebyshevDistance(const FGridPosition& StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);

	UFUNCTION(BlueprintCallable)
	static void GetChebyshevDistanceAtRanges(const FGridPosition& StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations);
	
	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	static void GetGridPositionsInRange(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);
};

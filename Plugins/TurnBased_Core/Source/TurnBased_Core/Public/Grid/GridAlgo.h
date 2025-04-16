// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridAlgo.generated.h"

class AGridUnit;
struct FGridMovement;
struct FGridPosition;

/**
 * source for all grid algorithm logic that should be used by grid based game modes
 */
UCLASS()
class TURNBASED_CORE_API UGridAlgo : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// measures distance by only allowing movement in horizontal and vertical steps, form a diamond-shaped region
	UFUNCTION(BlueprintCallable, Category="Grid Algo")
	static void GetManhattanDistance(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);

	UFUNCTION(BlueprintCallable, Category="Grid Algo")
	static void GetManhattanDistanceAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations);

	// forms a square, chess king's movement where diagonal costs the same...
	UFUNCTION(BlueprintCallable, Category="Grid Algo")
	static void GetChebyshevDistance(const FGridPosition& StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations);

	UFUNCTION(BlueprintCallable, Category="Grid Algo")
	static void GetChebyshevDistanceAtRanges(const FGridPosition& StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations);

	// other
	UFUNCTION(BlueprintCallable, Category="Grid Algo")
	static void CalculateGridMovement(TArray<FGridMovement>& OutMovement, const AGridUnit* GridUnit);
};

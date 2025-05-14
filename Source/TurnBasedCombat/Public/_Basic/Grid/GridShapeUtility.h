// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridShapeUtility.generated.h"


USTRUCT(BlueprintType)
struct FShapeConfiguration
{
	GENERATED_BODY()
	
	FShapeConfiguration() {}
	FShapeConfiguration(const TArray<FGridPosition>& GridPositions) : ShapeConfiguration(GridPositions) {}
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FGridPosition> ShapeConfiguration;
};


/**
 * map out different shapes on an XY grid
 */
UCLASS()
class TURNBASEDCOMBAT_API UGridShapeUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void GetConfigurationConnectFour(TArray<FShapeConfiguration>& Configurations, bool bRow = true, bool bCol = true, bool bDiagonal = true);
	
	UFUNCTION(BlueprintCallable)
	static void CheckConfiguration(
		const FGridPosition StartingPosition, const TArray<FGridPosition>& GridPositions,
		const TArray<FShapeConfiguration>& ConfigurationsToCheck, TArray<FShapeConfiguration>& OutSuccessfulConfigurations);
};

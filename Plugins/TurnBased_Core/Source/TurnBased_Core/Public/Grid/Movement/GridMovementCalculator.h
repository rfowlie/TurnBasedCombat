// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridMovementCalculator.generated.h"

class AGridTile;
class IGridTileCostPolicy;
class AGridUnit;
class UGridTileCostPolicy;
class UGridTileAdjacencyPolicy;
class UGridUnitMovementPolicy;


USTRUCT(BlueprintType)
struct FGridMovementResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTile> GridTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UObject> ResourceCost = nullptr;
};

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UGridMovementCalculator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MustImplement = "GridTileAdjacencyPolicy") )
	TSubclassOf<UObject> AdjacencyPolicy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MustImplement = "GridTileCostPolicy") )
	TSubclassOf<UObject> CostPolicy;
	
};

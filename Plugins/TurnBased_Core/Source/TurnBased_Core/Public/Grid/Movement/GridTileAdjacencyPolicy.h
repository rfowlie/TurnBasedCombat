// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridTileAdjacencyPolicy.generated.h"

class AGridTile;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGridTileAdjacencyPolicy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASED_CORE_API IGridTileAdjacencyPolicy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetAdjacent(const AGridTile* From, TArray<AGridTile*>& OutTiles);
};

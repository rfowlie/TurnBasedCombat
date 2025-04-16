// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridTileCostPolicy.generated.h"

class AGridUnit;
class AGridTile;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGridTileCostPolicy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASED_CORE_API IGridTileCostPolicy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool CanMoveTo(const AGridTile* From, const AGridTile* To, const AGridUnit* Unit) const;

	UFUNCTION(BlueprintNativeEvent)
	UObject* CloneWithMoveApplied(const AGridTile* From, const AGridTile* To, const AGridUnit* Unit) const;

	UFUNCTION(BlueprintNativeEvent)
	bool IsBetterThan(const UObject* OtherResource) const;

	// new
	// UFUNCTION(BlueprintNativeEvent)
	// bool Initialize(const AGridUnit* GridUnit, const AGridTile* Tile);
	 
};

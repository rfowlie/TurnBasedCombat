// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridUnitMovementPolicy.generated.h"

class AGridTile;
class AGridUnit;

// This class does not need to be modified.
UINTERFACE()
class UGridUnitMovementPolicy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASED_CORE_API IGridUnitMovementPolicy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanMoveToTile(const AGridUnit* Unit, const AGridTile* From, const AGridTile* To) const;
};

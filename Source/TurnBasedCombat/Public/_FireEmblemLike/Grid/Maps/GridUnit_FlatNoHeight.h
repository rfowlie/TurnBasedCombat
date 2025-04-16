// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridUnit_FlatNoHeight.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGridUnit_FlatNoHeight : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDCOMBAT_API IGridUnit_FlatNoHeight
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (Categories = "Grid User"))
	int32 GetAvailableMovement() const;
	
};

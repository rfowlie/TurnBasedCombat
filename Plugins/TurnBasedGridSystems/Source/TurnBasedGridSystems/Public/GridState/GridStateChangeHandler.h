// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridStateChangeHandler.generated.h"

struct FGridStatePayload;
struct FGridStateChangeHandlerResults;


// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UGridStateChangeHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDGRIDSYSTEMS_API IGridStateChangeHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HandleGridStateChanged(const FGridStatePayload& Payload, FGridStateChangeHandlerResults& Results);
};

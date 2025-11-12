// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "GigafireUnitHandler.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGigafireUnitHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GIGAFIRE_API IGigafireUnitHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FGameplayTag GetUnitPlayerTag();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ObjectPoolEnable();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ObjectPoolDisable();
	
};

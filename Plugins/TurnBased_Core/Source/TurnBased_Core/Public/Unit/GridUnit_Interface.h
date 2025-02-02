// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTags.h"
#include "GridUnit_Interface.generated.h"

struct FGameplayTag;


// This class does not need to be modified.
UINTERFACE()
class UGridUnit_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASED_CORE_API IGridUnit_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (Categories = "Faction"))
	FGameplayTag GetFaction() const;
	
	UFUNCTION(BlueprintNativeEvent)
	FGameplayTag GetFactionOther() const;

};

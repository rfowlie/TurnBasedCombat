// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "UnitTemplateDataAsset.h"
#include "GigafireInformationHandler.generated.h"

struct FUnitLoadData;
// struct FEmblemData;

// This class does not need to be modified.
UINTERFACE()
class UGigafireInformationHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GIGAFIRE_API IGigafireInformationHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<FGameplayTag> GetPlayerUnitTags();

};

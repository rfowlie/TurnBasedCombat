// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmblemSystemStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EmblemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UEmblemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Utilities|Guid")
	static FGuid MakeUniqueGuid();
	
};

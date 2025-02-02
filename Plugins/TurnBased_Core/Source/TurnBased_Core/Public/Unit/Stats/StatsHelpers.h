// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatsHelpers.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UStatsHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static int32 GetMinLevel() { return 1; }
	
	UFUNCTION(BlueprintCallable)
	static int32 GetMaxLevel() { return 99; }

	UFUNCTION(BlueprintCallable)
	static float GetMaxLevelFloat() { return 99.f; }
};

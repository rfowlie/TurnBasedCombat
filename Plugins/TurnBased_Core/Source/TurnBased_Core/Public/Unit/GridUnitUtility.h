// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridUnit.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridUnitUtility.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UGridUnitUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GridUnitUtility")
	static bool RegisterGridUnit(AGridUnit* InGridUnit);
	
	UFUNCTION(BlueprintCallable, Category = "GridUnitUtility")
	static bool UnregisterGridUnit(AGridUnit* InGridUnit);
};

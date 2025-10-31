// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnitTemplateUtility.generated.h"

struct FUnitSaveData;
struct FUnitLoadData;

/**
 * 
 */
UCLASS()
class GIGAFIRE_API UUnitTemplateUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Utilities|Guid")
	static FGuid MakeUniqueGuid();
	
	UFUNCTION(BlueprintCallable)
	static void MakeUnitSaveData(const FUnitLoadData& InLoadData, FUnitSaveData& OutSaveData);
	
	UFUNCTION(BlueprintCallable)
	static void MakeUnitLoadData(const FUnitSaveData& InSaveData, FUnitLoadData& OutLoadData);
};

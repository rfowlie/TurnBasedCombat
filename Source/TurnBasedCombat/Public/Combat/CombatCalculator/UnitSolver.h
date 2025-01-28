// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "UnitSolver.generated.h"

class AGridUnit;
class UGridManager;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UUnitSolver : public UObject
{
	GENERATED_BODY()

public:
	UUnitSolver();
	
	UFUNCTION(BlueprintCallable)
	virtual void GetUnitSolved(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits);
	
};

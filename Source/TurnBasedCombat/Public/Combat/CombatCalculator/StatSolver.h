// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatSolver.generated.h"


class UGridManager;
class AGridUnit;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UStatSolver : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	int32 GetStat(UGridManager* GridManager, AGridUnit* Instigator, AGridUnit* Target);
};

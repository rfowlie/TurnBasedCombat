﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "UObject/Object.h"
#include "CombatEventWrapper.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatEvent);

/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASED_CORE_API UCombatEventWrapper : public UObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly)
	FCombatSnapshot_Advanced InstigatorSnapShotAdvanced;

	UFUNCTION(BlueprintCallable)
	void CalculateDamage();
};

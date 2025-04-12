// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FireEmblemCombatStructs.h"
#include "FireEmblemCombatGamplayAbilityPayload.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UFireEmblemCombatGamplayAbilityPayload : public UObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly)
	FFireEmblemStatsAdvanced InstigatorSnapShotAdvanced;

	UFUNCTION(BlueprintCallable)
	void CalculateDamage();
};

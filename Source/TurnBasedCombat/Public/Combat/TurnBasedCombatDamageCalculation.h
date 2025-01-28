// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TurnBasedCombatDamageCalculation.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API UTurnBasedCombatDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UTurnBasedCombatDamageCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition AttributeCaptureDefinition_Strength;
	FGameplayEffectAttributeCaptureDefinition AttributeCaptureDefinition_Defence;
	// FGameplayEffectAttributeCaptureDefinition AttributeCaptureDefinition_Health;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ModMagnitudeCalculation_Damage.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UModMagnitudeCalculation_Damage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UModMagnitudeCalculation_Damage();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	// virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
    
protected:
    	FGameplayEffectAttributeCaptureDefinition AttributeCaptureDefinition_Strength;
    	FGameplayEffectAttributeCaptureDefinition AttributeCaptureDefinition_Defence;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatData.h"
#include "UObject/Interface.h"
#include "CombatEvaluator.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UCombatEvaluator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASED_CORE_API ICombatEvaluator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation, const FCombatPrediction& InCombatPrediction);
	
	// UFUNCTION(BlueprintCallable)
	// virtual float HitChanceInstigator() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float HitChanceTarget() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float CriticalChanceInstigator() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float CriticalChanceTarget() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float HealthChangePercentageInstigator() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float HealthChangePercentageTarget() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float DefeatedInstigator() const;
	// UFUNCTION(BlueprintCallable)
	// virtual float DefeatedTarget() const;
};

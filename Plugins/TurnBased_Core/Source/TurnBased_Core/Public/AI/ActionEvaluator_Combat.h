// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionEvaluator.h"
#include "CombatEvaluator.h"
#include "Combat/CombatData.h"
#include "UObject/Object.h"
#include "ActionEvaluator_Combat.generated.h"


class UCombatCalculator_Basic;

/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASED_CORE_API UActionEvaluator_Combat : public UObject, public IActionEvaluator, public ICombatEvaluator
{
	GENERATED_BODY()

	// UActionEvaluator_Combat();

public:
	// static UActionEvaluator_Combat* Create(
	// 	const FCombatPrediction& CombatPrediction);

	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAction() override;
	UFUNCTION(BlueprintCallable)
	virtual float EvaluateAction() override;
	UFUNCTION(BlueprintCallable)
	virtual void GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation, const FCombatPrediction& InCombatPrediction) override;

protected:
	// UPROPERTY()
	// FCombatPrediction CombatPrediction;

	// QUESTIONS
	// we need to provide a number of metrics by which a unit class/unit status can multiply its weights against
	// ideally all these questions will become an interface that another class can implement that will be assigned to a unit in editor
	// all questions should return a value between 0-1
	
	float HitChanceInstigator(const FCombatPrediction& InCombatPrediction) const;
	float HitChanceTarget(const FCombatPrediction& InCombatPrediction) const;
	float CriticalChanceInstigator(const FCombatPrediction& InCombatPrediction) const;
	float CriticalChanceTarget(const FCombatPrediction& InCombatPrediction) const;
	float HealthChangePercentageInstigator(const FCombatPrediction& InCombatPrediction) const;
	float HealthChangePercentageTarget(const FCombatPrediction& InCombatPrediction) const;
	float DefeatedInstigator(const FCombatPrediction& InCombatPrediction) const;
	float DefeatedTarget(const FCombatPrediction& InCombatPrediction) const;
};

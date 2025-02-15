// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionEvaluator.h"
#include "CombatEvaluator.h"
#include "Combat/CombatData.h"
#include "UObject/Object.h"
#include "ActionBehaviour_Combat.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class TURNBASED_CORE_API UActionBehaviour_Combat : public UObject, public ICombatEvaluator
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	 virtual void GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation, const FCombatPrediction& InCombatPrediction) override;
};

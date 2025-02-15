// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActionBehaviour_Combat.h"

void UActionBehaviour_Combat::GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation,
	const FCombatPrediction& InCombatPrediction)
{
	ICombatEvaluator::GetCombatEvaluation(OutCombatEvaluation, InCombatPrediction);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Components/GridUnitBehaviourComponent.h"



UGridUnitBehaviourComponent::UGridUnitBehaviourComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGridUnitBehaviourComponent::GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation,
                                                      const FCombatPrediction& InCombatPrediction)
{
	switch(CombatBehaviour)
	{
	case ECombatBehaviour::Neutral:
		GetCombatEvaluation_Neutral(OutCombatEvaluation);
		break;
	case ECombatBehaviour::Aggressive:
		GetCombatEvaluation_Aggressive(OutCombatEvaluation);
		break;
	case ECombatBehaviour::Conservative:
		GetCombatEvaluation_Conservative(OutCombatEvaluation);
		break;
	default: ;
	}
}

void UGridUnitBehaviourComponent::GetCombatEvaluation_Neutral(FCombatEvaluation& CombatEvaluation)
{
	// no change
}

void UGridUnitBehaviourComponent::GetCombatEvaluation_Aggressive(FCombatEvaluation& CombatEvaluation)
{
	// move concerned about dealing damage than taking...
	CombatEvaluation.HealthChangePercentageInstigator = 0.5f;
	CombatEvaluation.HealthChangePercentageTarget = 2.0f;
}

void UGridUnitBehaviourComponent::GetCombatEvaluation_Conservative(FCombatEvaluation& CombatEvaluation)
{
	// less concerned about dealing damage than taking...
	CombatEvaluation.HealthChangePercentageInstigator = 2.0f;
	CombatEvaluation.HealthChangePercentageTarget = 0.5f;
}



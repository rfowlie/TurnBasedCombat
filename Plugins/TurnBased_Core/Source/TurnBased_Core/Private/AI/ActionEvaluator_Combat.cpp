// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActionEvaluator_Combat.h"

// UActionEvaluator_Combat* UActionEvaluator_Combat::Create(const FCombatPrediction& CombatPrediction)
// {
// 	UActionEvaluator_Combat* Object = NewObject<UActionEvaluator_Combat>();
// 	Object->CombatPrediction = CombatPrediction;
// 	
// 	return Object;
// }

void UActionEvaluator_Combat::ExecuteAction()
{
	IActionEvaluator::ExecuteAction();
}

float UActionEvaluator_Combat::EvaluateAction()
{
	// FCombatEvaluation ObjectiveEvaluation;
	// GetCombatEvaluation(ObjectiveEvaluation);
	// FCombatEvaluation UnitEvaluation;
	// CombatPrediction.CombatInformation.InstigatorUnit->CombatBehaviourComponent

	return 1.0f;
}

void UActionEvaluator_Combat::GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation,
	const FCombatPrediction& InCombatPrediction)
{
	// CombatPrediction = InCombatPrediction;
	OutCombatEvaluation.HitChanceInstigator = HitChanceInstigator(InCombatPrediction);
	OutCombatEvaluation.HitChanceTarget = HitChanceTarget(InCombatPrediction);
	OutCombatEvaluation.CriticalChanceInstigator = CriticalChanceInstigator(InCombatPrediction);
	OutCombatEvaluation.CriticalChanceTarget = CriticalChanceTarget(InCombatPrediction);
	OutCombatEvaluation.HealthChangePercentageInstigator = HealthChangePercentageInstigator(InCombatPrediction);
	OutCombatEvaluation.HealthChangePercentageTarget = HealthChangePercentageTarget(InCombatPrediction);
	OutCombatEvaluation.DefeatedInstigator = DefeatedInstigator(InCombatPrediction);
	OutCombatEvaluation.DefeatedTarget = DefeatedTarget(InCombatPrediction);
}

float UActionEvaluator_Combat::HitChanceInstigator(const FCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.InstigatorSnapShotAdvanced.HitChance / 100.f, 1.5f);
}

float UActionEvaluator_Combat::HitChanceTarget(const FCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.TargetSnapshotAdvanced.HitChance / 100.f, 1.5f);
}

float UActionEvaluator_Combat::CriticalChanceInstigator(const FCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.InstigatorSnapShotAdvanced.CriticalChance / 100.f, 0.7);
}

float UActionEvaluator_Combat::CriticalChanceTarget(const FCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.TargetSnapshotAdvanced.CriticalChance / 100.f, 0.7);
}

float UActionEvaluator_Combat::HealthChangePercentageInstigator(const FCombatPrediction& InCombatPrediction) const
{
	if (InCombatPrediction.InstigatorSnapshotBasic.Health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UActionEvaluator_Combat::HealthChangePercentageInstigator - health zero"))
		return 0.f;
	}
	return InCombatPrediction.TargetSnapshotAdvanced.DamageDealt / InCombatPrediction.InstigatorSnapshotBasic.Health;
}

float UActionEvaluator_Combat::HealthChangePercentageTarget(const FCombatPrediction& InCombatPrediction) const
{
	if (InCombatPrediction.TargetSnapshotBasic.Health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ActionEvaluator_Combat::HealthChangePercentageTarget - health zero"))
		return 0.f;
	}
	return InCombatPrediction.InstigatorSnapShotAdvanced.DamageDealt / InCombatPrediction.TargetSnapshotBasic.Health;
}

float UActionEvaluator_Combat::DefeatedInstigator(const FCombatPrediction& InCombatPrediction) const
{
	return InCombatPrediction.TargetSnapshotAdvanced.DamageDealt < InCombatPrediction.InstigatorSnapshotBasic.Health ?
		1.0f : InCombatPrediction.TargetSnapshotAdvanced.HitChance;
}

float UActionEvaluator_Combat::DefeatedTarget(const FCombatPrediction& InCombatPrediction) const
{
	return InCombatPrediction.InstigatorSnapShotAdvanced.DamageDealt < InCombatPrediction.TargetSnapshotBasic.Health ?
		1.0f : InCombatPrediction.InstigatorSnapShotAdvanced.HitChance;
}

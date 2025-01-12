// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TurnBasedCombatDamageCalculation.h"
#include "Grid/Unit/GridUnitAttributeSet.h"


UTurnBasedCombatDamageCalculation::UTurnBasedCombatDamageCalculation()
{
	// FGameplayEffectAttributeCaptureDefinition Strength;
	AttributeCaptureDefinition_Strength.AttributeToCapture = UGridUnitAttributeSet::GetStrengthAttribute();
	AttributeCaptureDefinition_Strength.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttributeCaptureDefinition_Strength.bSnapshot = false;	
	RelevantAttributesToCapture.Add(AttributeCaptureDefinition_Strength);

	// FGameplayEffectAttributeCaptureDefinition Defence;
	AttributeCaptureDefinition_Defence.AttributeToCapture = UGridUnitAttributeSet::GetDefenceAttribute();
	AttributeCaptureDefinition_Defence.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AttributeCaptureDefinition_Defence.bSnapshot = false;
	RelevantAttributesToCapture.Add(AttributeCaptureDefinition_Defence);

	// FGameplayEffectAttributeCaptureDefinition Health;
	// AttributeCaptureDefinition_Health.AttributeToCapture = UGridUnitAttributeSet::GetHealthAttribute();
	// AttributeCaptureDefinition_Health.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// AttributeCaptureDefinition_Health.bSnapshot = false;
	// RelevantAttributesToCapture.Add(AttributeCaptureDefinition_Health);
}

void UTurnBasedCombatDamageCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (!SourceASC || !TargetASC)
	{
		return; // Invalid source or target
	}

	// SourceASC->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	// TargetASC->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	float Damage = TargetASC->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute()) -
		SourceASC->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	Damage = FMath::Max(0, Damage);

	// Capture source's strength
	float Strength = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeCaptureDefinition_Strength, FAggregatorEvaluateParameters(), Strength);

	// Capture target's defense
	float Defense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeCaptureDefinition_Defence, FAggregatorEvaluateParameters(), Defense);

	// Calculate raw damage
	float RawDamage = Strength - Defense;
	RawDamage = FMath::Max(0.0f, RawDamage); // Ensure damage is not less than zero

	// Apply the damage as a negative health delta
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UGridUnitAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -RawDamage));

}

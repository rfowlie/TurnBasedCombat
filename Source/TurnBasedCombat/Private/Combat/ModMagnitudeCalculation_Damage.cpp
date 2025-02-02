// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/ModMagnitudeCalculation_Damage.h"

#include "GameplayEffectExecutionCalculation.h"
#include "Unit/GridUnitAttributeSet.h"

UModMagnitudeCalculation_Damage::UModMagnitudeCalculation_Damage()
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
}

float UModMagnitudeCalculation_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	// const FGameplayTagContainer* TagsSource = Spec.CapturedSourceTags.GetAggregatedTags();
	// const FGameplayTagContainer* TagsTarget = Spec.CapturedTargetTags.GetAggregatedTags();

	// Do we need this? What does it do?
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Source_Strength = 0.f;
	GetCapturedAttributeMagnitude(AttributeCaptureDefinition_Strength, Spec, EvaluationParameters, Source_Strength);
	float Target_Defence = 0.f;
	GetCapturedAttributeMagnitude(AttributeCaptureDefinition_Defence, Spec, EvaluationParameters, Target_Defence);

	return FMath::Max(0, Source_Strength - Target_Defence);
	
}
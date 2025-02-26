// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatFlowHelper.h"

UCombatFlowHelper::UCombatFlowHelper()
{
}

UCombatFlowHelper* UCombatFlowHelper::Create(
	const FCombatPrediction& Prediction, UGameEventTaskManager* InPreAttackEventManager)
{
	UCombatFlowHelper* Object = NewObject<UCombatFlowHelper>();
	Object->CombatPrediction = Prediction;
	// Object->PreAttackEvent = InPreAttackEventManager;

	return Object;
}

void UCombatFlowHelper::Start()
{
	// CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	// CombatPrediction.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);

	// CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->Waitgame
	CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddUObject(
		this, &ThisClass::OnInstigatorAbilityComplete);
}

void UCombatFlowHelper::OnInstigatorAbilityComplete(const FAbilityEndedData& AbilityEndedData)
{
}

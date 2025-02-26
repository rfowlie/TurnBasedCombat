// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/GridUnitGameplayEffect.h"
#include "Unit/GridUnitAttributeSet.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Set_Unit_Health, "TBCore.SetByCaller.Unit.Health");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Set_Unit_Movement, "TBCore.SetByCaller.Unit.Movement");


UGridUnitGameplayEffect::UGridUnitGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Damage Modifier: This will subtract damage from Health.
	{
		FGameplayModifierInfo GameplayModifier;
		
		// Assuming Health is decreased by damage; use an additive modifier.
		GameplayModifier.Attribute = UGridUnitAttributeSet::GetHealthAttribute();
		GameplayModifier.ModifierOp = EGameplayModOp::Additive;

		// Set the magnitude calculation type to "Set By Caller"
		// Set the tag that will be used to set this magnitude dynamically.
		FSetByCallerFloat SetByCallerFloat = FSetByCallerFloat();
		SetByCallerFloat.DataTag = TAG_TBCore_Set_Unit_Health;		
		GameplayModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);

		Modifiers.Add(GameplayModifier);
	}

	// Movement Cost Modifier: This will add a movement cost value.
	{
		FGameplayModifierInfo GameplayModifier;
		
		// Assuming Health is decreased by damage; use an additive modifier.
		GameplayModifier.Attribute = UGridUnitAttributeSet::GetMovementAttribute();
		GameplayModifier.ModifierOp = EGameplayModOp::Additive;

		// Set the magnitude calculation type to "Set By Caller"
		// Set the tag that will be used to set this magnitude dynamically.
		FSetByCallerFloat SetByCallerFloat = FSetByCallerFloat();
		SetByCallerFloat.DataTag = TAG_TBCore_Set_Unit_Movement;
		GameplayModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);

		Modifiers.Add(GameplayModifier);
	}
}

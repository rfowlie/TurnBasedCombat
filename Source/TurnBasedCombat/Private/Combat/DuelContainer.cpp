// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/DuelContainer.h"

#include "Grid/Unit/GridUnit.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Duel_Instigator_Damage, "Duel.Instigator.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Duel_Instigator_Accuracy, "Duel.Instigator.Accuracy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Duel_Target_Damage, "Duel.Target.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Duel_Target_Accuracy, "Duel.Target.Accuracy");


UDuelContainer::UDuelContainer(): InstigatorUnit(nullptr), InstigatorTile(nullptr), TargetUnit(nullptr),
                                  TargetTile(nullptr)
{
}

UDuelContainer* UDuelContainer::CreateContainer(AGridUnit* InInstigatorUnit, AGridTile* InInstigatorTile,
                                                AGridUnit* InTargetUnit, AGridTile* InTargetTile)
{
	UDuelContainer* CombatProxy = NewObject<UDuelContainer>();
	CombatProxy->InstigatorUnit = InInstigatorUnit;
	CombatProxy->InstigatorTile = InInstigatorTile;
	CombatProxy->TargetUnit = InTargetUnit;
	CombatProxy->TargetTile = InTargetTile;

	return CombatProxy;
}

// return calculation for all exposes tags
FString UDuelContainer::GetDuelAttribute(FGameplayTag InTag) const
{
	// looks sort of sloppy, but let's just use it for now...
	FString Output;
	if (InTag == TAG_Duel_Instigator_Damage)
	{
		Output = FString::FromInt(GetInstigatorDamage());
	}
	else if (InTag == TAG_Duel_Instigator_Accuracy)
	{
		Output = FString::FromInt(GetInstigatorHitPercentage());
	}
	else if (InTag == TAG_Duel_Target_Damage)
	{
		Output = FString::FromInt(GetTargetDamage());
	}
	else if (InTag == TAG_Duel_Target_Accuracy)
	{
		Output = FString::FromInt(GetTargetHitPercentage());
	}

	return Output;
}

int32 UDuelContainer::GetInstigatorDamage() const
{
	int32 OutValue = 0;
	if (InstigatorUnit && TargetUnit)
	{
		OutValue = InstigatorUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute()) -
			TargetUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	}

	return OutValue;
}

int32 UDuelContainer::GetInstigatorHitPercentage() const
{
	return 100;
}

int32 UDuelContainer::GetTargetDamage() const
{
	int32 OutValue = 0;
	if (InstigatorUnit && TargetUnit)
	{
		OutValue = TargetUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute()) -
			InstigatorUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	}

	return FMath::Max(0, OutValue);
}

int32 UDuelContainer::GetTargetHitPercentage() const
{
	return 100;
}

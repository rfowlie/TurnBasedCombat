// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/DuelContainer.h"

#include "Grid/Unit/GridUnit.h"

UDuelContainer::UDuelContainer()
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

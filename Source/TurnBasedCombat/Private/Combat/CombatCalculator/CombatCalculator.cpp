// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/CombatCalculator.h"

#include "Combat/CombatCalculator/WeaponSolver.h"
#include "Grid/Manager/GridManager.h"
#include "Grid/Tile/GridTile.h"
#include "Grid/Unit/GridUnit.h"


void UCombatCalculator::SetEnabled() const
{
	if (OnEnabled.IsBound()) { OnEnabled.Broadcast(); }
}

void UCombatCalculator::SetDisabled() const
{
	if (OnDisabled.IsBound()) { OnDisabled.Broadcast(); }
}

void UCombatCalculator::SetInstigator(AGridUnit* Unit)
{
	if (IsValid(Unit))
	{
		InstigatorUnit = Unit;
		CalculateCombatSnapshot(InstigatorUnit, InstigatorTile, InstigatorSnapshot);
		if (OnInstigatorChanged.IsBound()) { OnInstigatorChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetTarget(AGridUnit* Unit)
{
	if (IsValid(Unit))
	{
		TargetUnit = Unit;
		CalculateCombatSnapshot(TargetUnit, TargetTile, TargetSnapshot);
		if (OnTargetChanged.IsBound()) { OnTargetChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetInstigatorTile(AGridTile* Tile)
{
	if (IsValid(Tile))
	{
		InstigatorTile = Tile;
		CalculateCombatSnapshot(InstigatorUnit, InstigatorTile, InstigatorSnapshot);
		if (OnInstigatorTileChanged.IsBound()) { OnInstigatorTileChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetTargetTile(AGridTile* Tile)
{
	if (IsValid(Tile) && TargetTile != Tile)
	{
		TargetTile = Tile;
		CalculateCombatSnapshot(TargetUnit, TargetTile, TargetSnapshot);
		if (OnTargetTileChanged.IsBound()) { OnTargetTileChanged.Broadcast(); }
	}
}

int32 UCombatCalculator::CalculateAttackSpeed(AGridUnit* Unit) const
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	FWeaponTraits WeaponTraits;
	GetWeapon(WeaponTraits, Unit->GetEquippedWeapon());
	return Speed - FMath::Max(0, WeaponTraits.Weight - Speed);
}

int32 UCombatCalculator::CalculateHitRate(AGridUnit* Unit) const
{
	FWeaponTraits WeaponTraits;
	GetWeapon(WeaponTraits, Unit->GetEquippedWeapon());
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill*2 + Luck + WeaponTraits.HitRate;
}

int32 UCombatCalculator::CalculateAvoid(AGridUnit* Unit, AGridTile* Tile) const
{
	return Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute()) +
		Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute()) / 2 +
		Tile->TerrainDataAsset->TerrainStats.Avoid;
}

int32 UCombatCalculator::CalculateCriticalRate(AGridUnit* Unit) const
{
	FWeaponTraits WeaponTraits;
	GetWeapon(WeaponTraits, Unit->GetEquippedWeapon());
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill/2 + Luck/4 + WeaponTraits.CriticalBonus;
}

int32 UCombatCalculator::CalculateCriticalAvoid(AGridUnit* Unit, AGridTile* Tile) const
{
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	const int32 TerrainAvoid = Tile->TerrainDataAsset->TerrainStats.Avoid;
	return Luck + TerrainAvoid/4;
}

void UCombatCalculator::CalculateCombatSnapshot(AGridUnit* Unit, AGridTile* Tile, FUnitCombatSnapshot& Snapshot) const
{
	Snapshot.Strength = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	Snapshot.Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	Snapshot.Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	Snapshot.Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	Snapshot.Defence = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	Snapshot.Resistance = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetResistanceAttribute());
	Snapshot.Movement = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetMovementAttribute());
	Snapshot.Constitution = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetConstitutionAttribute());
	Snapshot.AttackSpeed = CalculateAttackSpeed(Unit);
	Snapshot.HitRate = CalculateAttackSpeed(Unit);
	Snapshot.Avoid = CalculateAvoid(Unit, Tile);
	Snapshot.CriticalRate = CalculateCriticalRate(Unit);
	Snapshot.CriticalAvoid = CalculateCriticalAvoid(Unit, Tile);
}

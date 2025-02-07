// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator_Basic.h"
#include "Combat/CombatData.h"
#include "Grid/GridHelper.h"
#include "Grid/GridWorldSubsystem.h"
#include "Tile/GridTile.h"
#include "Unit/GridUnit.h"


void UCombatCalculator_Basic::GetCombatOutcome(
	FCombatSnapshot_Outcome& Outcome, AGridUnit* InstigatorUnit, AGridUnit* TargetUnit)
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }

	FCombatSnapshot_Basic InstigatorSnapshot;
	GetUnitSnapshot(InstigatorSnapshot, InstigatorUnit);
	FCombatSnapshot_Basic TargetSnapshot;
	GetUnitSnapshot(TargetSnapshot, TargetUnit);

	// THE HARD WAY
	// check if target can attack first (special attributes or skills, etc.)
	// do instigator attack
	// check if target still alive (sub damage from health)
	// check if target can attack back (weapon range correct, etc.)
	// do target attack
	// check if even hits, could also do partial hits...

	// THE EASY WAY
	// get rough outcomes for instigator->target & target->instigator
	FCombatSnapshot_Advanced InstigatorOutcome;
	GetAdvancedSnapshot(InstigatorOutcome, InstigatorSnapshot, TargetSnapshot);
	FCombatSnapshot_Advanced TargetOutcome;
	GetAdvancedSnapshot(TargetOutcome, TargetSnapshot, InstigatorSnapshot);
	
	Outcome.InstigatorSnapshot = InstigatorOutcome;
	Outcome.TargetSnapshot = TargetOutcome;

	// check how many attacks each unit can potentially get
	const int32 CombatRange = UGridHelper::CalculateGridPosition(InstigatorUnit).GetDistance(UGridHelper::CalculateGridPosition(TargetUnit));
	Outcome.InstigatorAttacks = InstigatorSnapshot.WeaponTraits.Range == CombatRange ? 1 : 0;
	Outcome.TargetAttacks = TargetSnapshot.WeaponTraits.Range == CombatRange ? 1 : 0;
	// DO FE IF ATTACK SPEED GREATER BY 4+
	Outcome.InstigatorAttacks *= InstigatorSnapshot.AttackSpeed - 4 > TargetSnapshot.Speed ? 2 : 1;
	Outcome.TargetAttacks *= TargetSnapshot.AttackSpeed - 4 > InstigatorSnapshot.Speed ? 2 : 1;
}

void UCombatCalculator_Basic::GetUnitSnapshot(FCombatSnapshot_Basic& OutSnapshot, AGridUnit* InGridUnit) const
{
	if (!IsValid(InGridUnit)) { return; }

	AGridTile* Tile = nullptr;
	if (UGridWorldSubsystem* GridSubsystem = InGridUnit->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Tile = GridSubsystem->GetGridTileOfUnit(InGridUnit);
	}

	if (!IsValid(Tile)) { return; }
	
	GetWeapon(OutSnapshot.WeaponTraits, InGridUnit->GetEquippedWeapon());
	OutSnapshot.Strength = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	OutSnapshot.Skill = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	OutSnapshot.Speed = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	OutSnapshot.Luck = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	OutSnapshot.Defence = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	OutSnapshot.Resistance = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetResistanceAttribute());
	OutSnapshot.Movement = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetMovementAttribute());
	OutSnapshot.Constitution = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetConstitutionAttribute());
	OutSnapshot.AttackSpeed = CalculateAttackSpeed(InGridUnit, OutSnapshot.WeaponTraits);
	OutSnapshot.HitRate = CalculateHitRate(InGridUnit, OutSnapshot.WeaponTraits);
	OutSnapshot.Avoid = CalculateAvoid(InGridUnit, Tile);
	OutSnapshot.CriticalRate = CalculateCriticalRate(InGridUnit, OutSnapshot.WeaponTraits);
	OutSnapshot.CriticalAvoid = CalculateCriticalAvoid(InGridUnit, Tile);
}

void UCombatCalculator_Basic::GetAdvancedSnapshot(
	FCombatSnapshot_Advanced& OutSnapshot, const FCombatSnapshot_Basic& InstigatorSnapshot, const FCombatSnapshot_Basic& TargetSnapshot)
{
	OutSnapshot.HitChance = FMath::Clamp(InstigatorSnapshot.HitRate - TargetSnapshot.Avoid, 0, 100);
	OutSnapshot.bHit = FMath::RandRange(0, 100) < OutSnapshot.HitChance;
	OutSnapshot.HealthChange = FMath::Clamp(InstigatorSnapshot.Strength + InstigatorSnapshot.WeaponTraits.Might - TargetSnapshot.Defence, 0, GetMaxDamage());
	OutSnapshot.CriticalChance = FMath::Clamp(0, 100, InstigatorSnapshot.CriticalRate - TargetSnapshot.CriticalAvoid);

}

int32 UCombatCalculator_Basic::CalculateAttackSpeed(const AGridUnit* Unit, FWeaponTraits& WeaponTraits)
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	return Speed - FMath::Max(0, WeaponTraits.Weight - Speed);
}

int32 UCombatCalculator_Basic::CalculateHitRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits)
{
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill*2 + Luck + WeaponTraits.HitRate;
}

int32 UCombatCalculator_Basic::CalculateAvoid(const AGridUnit* Unit, const AGridTile* Tile)
{
	return Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute()) +
		Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute()) / 2 +
		Tile->TerrainDataAsset->TerrainStats.Avoid;
}

int32 UCombatCalculator_Basic::CalculateCriticalRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits)
{
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill/2 + Luck/4 + WeaponTraits.CriticalBonus;
}

int32 UCombatCalculator_Basic::CalculateCriticalAvoid(const AGridUnit* Unit, const AGridTile* Tile)
{
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	const int32 TerrainAvoid = Tile->TerrainDataAsset->TerrainStats.Avoid;
	return Luck + TerrainAvoid/4;
}

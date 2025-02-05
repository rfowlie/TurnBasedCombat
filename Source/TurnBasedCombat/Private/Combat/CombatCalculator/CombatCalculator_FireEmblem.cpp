// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/CombatCalculator_FireEmblem.h"

#include "Combat/CombatCalculator/CombatCalculatorData.h"
#include "Combat/CombatCalculator/UnitSolver_FireEmblem.h"
#include "Grid/Manager/GridManager.h"
#include "Tile/GridTile.h"
#include "Unit/GridUnit.h"
#include "Unit/GridUnitAttributeSet.h"


UCombatCalculator_FireEmblem::UCombatCalculator_FireEmblem()
{
}

UCombatCalculator_FireEmblem* UCombatCalculator_FireEmblem::Create(
	UGridManager* InGridManager, AGridUnit* InstigatorUnit, AGridUnit* TargetUnit)
{
	UCombatCalculator_FireEmblem* CombatCalculator = NewObject<UCombatCalculator_FireEmblem>();
	CombatCalculator->UnitSolver = NewObject<UUnitSolver_FireEmblem>();
	CombatCalculator->WeaponSolver = NewObject<UWeaponSolver>();
	CombatCalculator->CalculateCombat(InGridManager, InstigatorUnit, TargetUnit);
	return CombatCalculator;
}

void UCombatCalculator_FireEmblem::CalculateCombat(
	UGridManager* InGridManager, AGridUnit* InstigatorUnit, AGridUnit* TargetUnit)
{
	GridManager = InGridManager;
	
	Instigator = InstigatorUnit;
	// UnitSolver->Solve(InstigatorAttributes, InstigatorTraits, GridManager, InstigatorUnit, TargetUnit);
	// WeaponSolver->Solve(InstigatorAttributes, InstigatorTraits, GridManager, InstigatorUnit, TargetUnit);
	InstigatorAttributes.Add(TAG_Combat_Stat_AttackSpeed, CalculateAttackSpeed(Instigator));
	InstigatorAttributes.Add(TAG_Combat_Stat_HitRate, CalculateHitRate(Instigator));
	InstigatorAttributes.Add(TAG_Combat_Stat_Avoid, CalculateAvoid(Instigator));

	
	Target = TargetUnit;
	// UnitSolver->Solve(TargetAttributes, TargetTraits, GridManager, TargetUnit, InstigatorUnit);
	// WeaponSolver->Solve(TargetAttributes, TargetTraits, GridManager, TargetUnit, InstigatorUnit);
	TargetAttributes.Add(TAG_Combat_Stat_AttackSpeed, CalculateAttackSpeed(Target));
	TargetAttributes.Add(TAG_Combat_Stat_HitRate, CalculateHitRate(Target));
	TargetAttributes.Add(TAG_Combat_Stat_Avoid, CalculateAvoid(Target));


	// calculate traits that depend on opponents information
	InstigatorAttributes.Add(
		TAG_Combat_Stat_Accuracy,
		FMath::Clamp(InstigatorAttributes[TAG_Combat_Stat_HitRate] - TargetAttributes[TAG_Combat_Stat_Avoid], 0, 100));
	InstigatorAttributes.Add(
			TAG_Combat_Stat_Accuracy,
			FMath::Clamp(TargetAttributes[TAG_Combat_Stat_HitRate] - InstigatorAttributes[TAG_Combat_Stat_Avoid], 0, 100));

}

int32 UCombatCalculator_FireEmblem::CalculateAttackSpeed(AGridUnit* Unit) const
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	FWeaponTraits WeaponTraits;
	WeaponSolver->GetWeaponTraits(WeaponTraits, Unit->GetEquippedWeapon());
	return Speed - FMath::Max(0, WeaponTraits.Weight - Speed);
}

int32 UCombatCalculator_FireEmblem::CalculateHitRate(AGridUnit* Unit) const
{
	FWeaponTraits WeaponTraits;
	WeaponSolver->GetWeaponTraits(WeaponTraits, Unit->GetEquippedWeapon());
	int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill * 2 + Luck + WeaponTraits.HitRate;
}


int32 UCombatCalculator_FireEmblem::CalculateAvoid(AGridUnit* Unit) const
{
	return Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute()) +
		Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute()) / 2 +
		GridManager->GetGridTileOfUnit(Unit)->TerrainDataAsset->TerrainStats.Avoid;
}
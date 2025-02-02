// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/UnitSolver_FireEmblem.h"

#include "Combat/CombatCalculator/CombatCalculatorData.h"
#include "Combat/CombatCalculator/WeaponSolver.h"
#include "Grid/Manager/GridManager.h"
#include "Tile/GridTile.h"
#include "Unit/GridUnit.h"

UUnitSolver_FireEmblem::UUnitSolver_FireEmblem()
{
}

void UUnitSolver_FireEmblem::GetUnitSolved(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits)
{
	// Super::GetUnitSolved(Attributes, Traits);
}

void UUnitSolver_FireEmblem::Solve(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
	const FCombatCalculatorPayload_FireEmblem& Payload)
{
	
}

void UUnitSolver_FireEmblem::Solve(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
	UGridManager* GridManager, AGridUnit* Instigator, AGridUnit* Target)
{
	// put all attributes from unit and weapon into map
	// add basic stats
	Attributes.Add(TAG_Combat_Stat_Strength, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute()));
	Attributes.Add(TAG_Combat_Stat_Skill, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute()));
	Attributes.Add(TAG_Combat_Stat_Speed, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute()));
	Attributes.Add(TAG_Combat_Stat_Luck, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute()));
	Attributes.Add(TAG_Combat_Stat_Defence, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute()));
	Attributes.Add(TAG_Combat_Stat_Resistance, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetResistanceAttribute()));
	Attributes.Add(TAG_Combat_Stat_Movement, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetMovementAttribute()));
	Attributes.Add(TAG_Combat_Stat_Constitution, Instigator->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetConstitutionAttribute()));

	UWeaponSolver* WeaponSolver = NewObject<UWeaponSolver>();
	WeaponSolver->Solve(Attributes, Traits, GridManager, Instigator, Target);

		
	// add calculated stats
	const float AttackSpeed = Attributes[TAG_Combat_Stat_Speed] -
		FMath::Max(0, Attributes[TAG_Weapon_Trait_Weight] - Attributes[TAG_Combat_Stat_Speed]);
	Attributes.Add(TAG_Combat_Stat_AttackSpeed, AttackSpeed);

	const float HitRate = Attributes[TAG_Combat_Stat_Skill] * 2 + Attributes[TAG_Combat_Stat_Luck] +
		Attributes[TAG_Weapon_Trait_Hit];
	Attributes.Add(TAG_Combat_Stat_HitRate, HitRate);
	
	const float Avoid = Attributes[TAG_Combat_Stat_Speed] + Attributes[TAG_Combat_Stat_Luck] / 2 +
		GridManager->GetGridTileOfUnit(Instigator)->TerrainDataAsset->TerrainStats.Avoid;
	Attributes.Add(TAG_Combat_Stat_Avoid, Avoid);

	// TODO: Accuracy is a weird one as it requires to know about opponent...
	// should move this into outer scope...
	// UUnitSolver_FireEmblem* TargetSolver = NewObject<UUnitSolver_FireEmblem>();
	// TargetSolver->Solve(Attributes, Traits, GridManager, Instigator, Target);
	// const float Accuracy = Attributes[TAG_Combat_Stat_Skill] +
	// 	TargetSolver->SolveAttribute(TAG_Combat_Stat_Avoid, TArray<FGameplayTag>(), GridManager, Target, Instigator);

	
}

int32 UUnitSolver_FireEmblem::SolveAttribute(FGameplayTag Attribute, TArray<FGameplayTag> Traits,
	UGridManager* GridManager, AGridUnit* Instigator, AGridUnit* Target)
{
	return 0;
}

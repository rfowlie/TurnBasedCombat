// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/WeaponSolver.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Trait_Might, "Weapon.Trait.Might");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Trait_Weight, "Weapon.Trait.Weight");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Trait_Hit, "Weapon.Trait.Hit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Trait_Range, "Weapon.Trait.Range");

UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Trait_Special_Light, "Weapon.Trait.Special.Light");


void UWeaponSolver::GetWeaponTraits(FWeaponTraits& WeaponTraits, const FGameplayTag InWeapon) const
{
	GetWeapon(WeaponTraits, InWeapon);
	for (auto SpecialTrait : WeaponTraits.SpecialTraits)
	{
		AddSpecialTrait(WeaponTraits, SpecialTrait);
	}
}

void UWeaponSolver::Solve(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
	const FCombatCalculatorPayload_FireEmblem& Payload)
{
}

void UWeaponSolver::Solve(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
	const UGridManager* GridManager, const AGridUnit* Instigator, const AGridUnit* Target)
{
}


// int32 UWeaponSolver::GetWeaponTrait(
// 	const FGameplayTag InWeapon, const FGameplayTag InTrait) const
// {
// 	FWeaponTraits WeaponTraits;
// 	GetWeapon(WeaponTraits, InWeapon);
// 	for (auto UniqueTrait : WeaponTraits.SpecialTraits)
// 	{
// 		if (UniqueTrait.Trait == InTrait)
// 		{
// 			UStatSolver* StatSolver = NewObject<UStatSolver>(GetWorld(), UniqueTrait.StatSolver);
// 			return StatSolver->GetStat(GridManager, Instigator, Target);
// 		}
// 	}
// 	
// 	if (InTrait == TAG_Weapon_Trait_Might)
// 	{
// 		return WeaponTraits.Might;
// 	}
// 	if (InTrait == TAG_Weapon_Trait_Weight)
// 	{
// 		return WeaponTraits.Weight;
// 	}
// 	if (InTrait == TAG_Weapon_Trait_Hit)
// 	{
// 		return WeaponTraits.Hit;
// 	}
// 	if (InTrait == TAG_Weapon_Trait_Range)
// 	{
// 		return WeaponTraits.Range;
// 	}
//
// 	return 0;
// }

// void UWeaponSolver::AddSpecialTrait(FWeaponTraits& WeaponTraits, FGameplayTag SpecialTrait) const
// {
// 	if (SpecialTrait == TAG_Weapon_Trait_Special_Light)
// 	{
// 		WeaponTraits.Hit += 10;
// 	}
// }

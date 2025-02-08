// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Item/Weapon.h"


int32 UWeapon::GetWeight() const
{
	return WeaponStats.Weight;
}

int32 UWeapon::GetEncumbrance() const
{
	return WeaponStats.Encumbrance;
}

int32 UWeapon::GetDurability() const
{
	return WeaponStats.Durability;
}

int32 UWeapon::GetRange() const
{
	return WeaponStats.Range;
}

int32 UWeapon::GetDamage() const
{
	return WeaponStats.Damage;
}

EWeaponDamageType UWeapon::GetWeaponDamageType() const
{
	return WeaponStats.WeaponDamageType;
}

int32 UWeapon::GetEffort() const
{
	return WeaponStats.Effort;
}

// TODO: ensures stats set only one time, is this what we want???
bool UWeapon::SetWeaponStats(const UWeaponDataAsset_OLD* WeaponDataAsset)
{
	if (!IsValid(WeaponDataAsset) || bWeaponsStatsSet) { return false; }

	WeaponStats = WeaponDataAsset->WeaponStats;
	bWeaponsStatsSet = true;
	return true;
}

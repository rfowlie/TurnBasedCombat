// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataAsset_OLD.h"
#include "UObject/Object.h"
#include "Weapon.generated.h"


/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UWeapon : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	bool bWeaponsStatsSet = false;
	
	UPROPERTY()
	FWeaponStats WeaponStats;
	
public:
	int32 GetWeight() const;
	int32 GetEncumbrance() const;
	int32 GetDurability() const;
	int32 GetRange() const;
	int32 GetDamage() const;
	EWeaponDamageType GetWeaponDamageType() const;
	int32 GetEffort() const;

	UFUNCTION()
	bool SetWeaponStats(const UWeaponDataAsset_OLD* WeaponDataAsset);
};

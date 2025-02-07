// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatData.h"
#include "Weapon/WeaponData.h"
#include "CombatCalculator_Basic.generated.h"

class AGridTile;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UCombatCalculator_Basic : public UObject
{
	GENERATED_BODY()

public:
	void GetCombatOutcome(FCombatSnapshot_Outcome& Outcome, AGridUnit* InstigatorUnit, AGridUnit* TargetUnit);

	// unit
	void GetUnitSnapshot(FCombatSnapshot_Basic& OutSnapshot, AGridUnit* InGridUnit) const;
	void GetAdvancedSnapshot(FCombatSnapshot_Advanced& OutSnapshot, const FCombatSnapshot_Basic& InstigatorSnapshot, const FCombatSnapshot_Basic& TargetSnapshot);

	// weapon
	UFUNCTION(BlueprintImplementableEvent)
	void GetWeapon(FWeaponTraits& WeaponTraits, FGameplayTag Weapon) const;

protected:
	static int32 GetMaxDamage() { return 60; }
	static int32 CalculateAttackSpeed(const AGridUnit* Unit, FWeaponTraits& WeaponTraits);
	static int32 CalculateHitRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits);
	static int32 CalculateAvoid(const AGridUnit* Unit, const AGridTile* Tile);
	static int32 CalculateCriticalRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits);
	static int32 CalculateCriticalAvoid(const AGridUnit* Unit, const AGridTile* Tile);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatData.h"
#include "Grid/GridStructs.h"
#include "Weapon/WeaponData.h"
#include "CombatCalculator_Basic.generated.h"

class UWeaponDataAsset;
class AGridTile;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API UCombatCalculator_Basic : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	void GetCombatOutcome(
		FCombatSnapshot_Outcome& Outcome, AGridUnit* InstigatorUnit, const FName WeaponName, AGridUnit* TargetUnit) const;

	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	TArray<AGridUnit*> GetCombatOrder(FCombatSnapshot_Outcome& Outcome);
	
	// unit
	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	void GetUnitSnapshotBasic(FCombatSnapshot_Basic& OutSnapshot, AGridUnit* InGridUnit, const FName WeaponName) const;
	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	void GetUnitSnapshotAdvanced(
		FCombatSnapshot_Advanced& OutSnapshot,
		const FCombatSnapshot_Basic& InstigatorSnapshot,
		const FCombatSnapshot_Basic& TargetSnapshot) const;

	// weapon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetWeapon(FWeaponTraits& WeaponTraits, const FGameplayTag& WeaponTag) const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	int32 GetWeaponRange(const FGameplayTag& WeaponTag) const;

	// weapon - new (using tags will create so many tags...)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UWeaponDataAsset* GetWeaponDataAsset();

	// weapon - name
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetWeaponByName(FWeaponContainer& WeaponContainer, const FName& WeaponName) const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetWeaponTraitsByName(FWeaponTraits& WeaponTraits, const FName& WeaponName) const;

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetWeaponRangesByName(const TArray<FName>& WeaponNames) const;
	
protected:
	static int32 GetMaxDamage() { return 60; }
	static int32 CalculateAttackSpeed(const AGridUnit* Unit, FWeaponTraits& WeaponTraits);
	static int32 CalculateHitRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits);
	static int32 CalculateAvoid(const FCombatSnapshot_Basic& Snapshot, const AGridUnit* Unit, const AGridTile* Tile);
	static int32 CalculateCriticalRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits);
	static int32 CalculateCriticalAvoid(const AGridUnit* Unit, const AGridTile* Tile);

	// AI
public:
	void CalculateCombatScore(FCombatScore& CombatScore) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCalculatorData.h"
#include "GameplayTagContainer.h"
#include "WeaponSolver.h"
#include "Grid/Tile/GridTile.h"
#include "UObject/Object.h"
#include "CombatCalculator.generated.h"

class UGridManager;
class AGridTile;
class AGridUnit;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatCalculatorEvent)

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API UCombatCalculator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnEnabled;
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnDisabled;
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnInstigatorChanged;
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnInstigatorTileChanged;
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnTargetChanged;
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnTargetTileChanged;
	
	void SetEnabled() const;
	void SetDisabled() const;
	void SetInstigator(AGridUnit* Unit);
	void SetTarget(AGridUnit* Unit);
	void SetInstigatorTile(AGridTile* Tile);
	void SetTargetTile(AGridTile* Tile);
	
	int32 CalculateAttackSpeed(AGridUnit* Unit) const;
	int32 CalculateHitRate(AGridUnit* Unit) const;
	int32 CalculateAvoid(AGridUnit* Unit, AGridTile* Tile) const;
	int32 CalculateCriticalRate(AGridUnit* Unit) const;
	int32 CalculateCriticalAvoid(AGridUnit* Unit, AGridTile* Tile) const;
	
	void CalculateCombatSnapshot(AGridUnit* Unit, AGridTile* Tile, FUnitCombatSnapshot& Snapshot) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetWeapon(FWeaponTraits& WeaponTraits, FGameplayTag Weapon) const;
	
	UPROPERTY()
	UGridManager* GridManager;
	UPROPERTY()
	AGridUnit* InstigatorUnit;
	UPROPERTY()
	AGridUnit* TargetUnit;
	UPROPERTY()
	AGridTile* InstigatorTile;
	UPROPERTY()
	AGridTile* TargetTile;
	UPROPERTY()
	FUnitCombatSnapshot InstigatorSnapshot;
	UPROPERTY()
	FUnitCombatSnapshot TargetSnapshot;

	
	// UPROPERTY()
	// TMap<FGameplayTag, int32>& AttributesInstigator;
	// UPROPERTY()
	// TMap<FGameplayTag, int32>& AttributesTarget;
	// UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	// const TMap<FGameplayTag, int32>& GetAttributesInstigator() const { return AttributesInstigator; }
	// UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	// const TMap<FGameplayTag, int32>& GetAttributesTarget() const { return AttributesTarget; }
	
};

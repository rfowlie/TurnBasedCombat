// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCalculatorData.h"
#include "GameplayTagContainer.h"
#include "WeaponSolver.h"
#include "Tile/GridTile.h"
#include "UObject/Object.h"
#include "CombatCalculator.generated.h"

class UGridManager;
class AGridTile;
class AGridUnit;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatCalculatorEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatCalculatorUnitEvent, AGridUnit*, GridUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatCalculatorTileEvent, AGridTile*, GridTile);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
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
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnCombatBegin;
	UPROPERTY(BlueprintAssignable, Category="CombatCalculator")
	FCombatCalculatorEvent OnCombatEnd;
	
	void SetEnabled() const;
	void SetDisabled() const;
	void SetInstigator(AGridUnit* Unit);
	UFUNCTION(BlueprintCallable)
	const AGridUnit* GetInstigatorUnit() const { return InstigatorUnit; }
	void SetTarget(AGridUnit* Unit);
	UFUNCTION(BlueprintCallable)
	const AGridUnit* GetTargetUnit() const { return TargetUnit; }
	void SetInstigatorTile(AGridTile* Tile);
	UFUNCTION(BlueprintCallable)
	const AGridTile* GetInstigatorTile() const { return InstigatorTile; }
	void SetTargetTile(AGridTile* Tile);
	UFUNCTION(BlueprintCallable)
	const AGridTile* GetTargetTile() const { return TargetTile; }

	TQueue<AGridUnit*> CombatOrder;
	UPROPERTY()
	TArray<AGridUnit*> CombatOrderHelper;
	void CalculateCombatOrder();
	const AGridUnit* GetNextCombatant();
	void InitiateCombat();
	UFUNCTION()
	void NextAttacker();
	UPROPERTY()
	bool bCombatLock = false;

protected:
	static int32 GetMaxDamage() { return 60; }
	int32 CalculateAttackSpeed(AGridUnit* Unit, FWeaponTraits& WeaponTraits) const;
	int32 CalculateHitRate(AGridUnit* Unit, FWeaponTraits& WeaponTraits) const;
	int32 CalculateAvoid(AGridUnit* Unit, AGridTile* Tile) const;
	int32 CalculateCriticalRate(AGridUnit* Unit, FWeaponTraits& WeaponTraits) const;
	int32 CalculateCriticalAvoid(AGridUnit* Unit, AGridTile* Tile) const;
	void CalculateCombatSnapshot_Internal(FUnitCombatSnapshot& OutSnapshot, AGridUnit* Unit);
	
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

public:
	void SetGridManager(UGridManager* InGridManager) { GridManager = InGridManager; }
	// for now... maybe later convert to tags???
	UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	const FUnitCombatSnapshot& GetCombatSnapshotInstigator() const { return InstigatorSnapshot; }
	UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	const FUnitCombatSnapshot& GetCombatSnapshotTarget() const { return TargetSnapshot; }


	// now we need to be able to execute combat in the correct order, ensuring not to do extra calls, etc.
	UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	bool PopCombatOutcome(FCombatOutcome& CombatOutcome);

	UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	void GetCombatOutcomes(TArray<FCombatOutcome>& CombatOutcomes);

protected:
	void CalculateCombatOutcome(
		FCombatOutcome& CombatOutcome, const FUnitCombatSnapshot& Instigator, const FUnitCombatSnapshot& Target) const;



	/////////////////////////////////////////////////////////////////////////////
	// UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	// void GetInstigatorInfo(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag>& Traits);
	// UPROPERTY()
	// TMap<FGameplayTag, int32>& AttributesInstigator;
	// UPROPERTY()
	// TMap<FGameplayTag, int32>& AttributesTarget;
	// UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	// const TMap<FGameplayTag, int32>& GetAttributesInstigator() const { return AttributesInstigator; }
	// UFUNCTION(BlueprintCallable, Category="CombatCalculator")
	// const TMap<FGameplayTag, int32>& GetAttributesTarget() const { return AttributesTarget; }
	
};

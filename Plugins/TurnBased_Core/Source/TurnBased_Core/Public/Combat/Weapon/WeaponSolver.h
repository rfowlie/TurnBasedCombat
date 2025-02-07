// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "WeaponData.h"
#include "WeaponSolver.generated.h"


// class UGridManager;
class AGridUnit;
class UStatSolver;

/**
 * For now do not worry about making generic
 * implement the logic for unique traits here, force BPs to implement get weapon
 * as it will most likely be easier to make a DT for them...
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API UWeaponSolver : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void GetWeaponTraits(FWeaponTraits& WeaponTraits, const FGameplayTag InWeapon) const;
	
	// UFUNCTION(BlueprintCallable)
	// virtual int32 GetWeaponTrait(const FGameplayTag InWeapon, const FGameplayTag InTrait) const;

	// virtual void Solve(
	// 	TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
	// 	const FCombatCalculatorPayload_FireEmblem& Payload);
	// virtual void Solve(
	// 	TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
	// 	const UGridManager* InGridManager, const AGridUnit* InInstigator, const AGridUnit* InTarget);
	//
	//
	// void SetGridManager(UGridManager* InGridManager) { GridManager = InGridManager; }
	void SetInstigator(AGridUnit* InInstigator) { Instigator = InInstigator; }
	void SetTarget(AGridUnit* InTarget) { Target = InTarget; }
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void GetWeapon(FWeaponTraits& WeaponTraits, FGameplayTag Weapon) const;

	// FOR NOW... just do here, can eventually use a DT or allow for custom setup
	UFUNCTION(BlueprintImplementableEvent)
	void AddSpecialTrait(FWeaponTraits& WeaponTraits, FGameplayTag SpecialTrait) const;
	
	// UPROPERTY()
	// UGridManager* GridManager;
	UPROPERTY()
	AGridUnit* Instigator;
	UPROPERTY()
	AGridUnit* Target;
};

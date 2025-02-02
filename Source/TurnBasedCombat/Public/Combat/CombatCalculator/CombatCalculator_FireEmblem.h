// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponSolver.h"
#include "Unit/GridUnit.h"
#include "UObject/Object.h"
#include "CombatCalculator_FireEmblem.generated.h"


class UGridManager;
class AGridUnit;
class UUnitSolver;
class UUnitSolver_FireEmblem;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatUpdate);

/**
 * 
 */

UCLASS()
class TURNBASEDCOMBAT_API UCombatCalculator_FireEmblem : public UObject
{
	GENERATED_BODY()

	UCombatCalculator_FireEmblem();

public:
	static UCombatCalculator_FireEmblem* Create(UGridManager* InGridManager, AGridUnit* InstigatorUnit, AGridUnit* TargetUnit);

protected:	
	UPROPERTY()
	UUnitSolver_FireEmblem* UnitSolver = nullptr;
	UPROPERTY()
	UWeaponSolver* WeaponSolver = nullptr;

	UPROPERTY()
	UGridManager* GridManager = nullptr;
	
	UPROPERTY()
	AGridUnit* Instigator = nullptr;
	UPROPERTY()
	TMap<FGameplayTag, int32> InstigatorAttributes;
	UPROPERTY()
	TArray<FGameplayTag> InstigatorTraits;
	
	UPROPERTY()
	AGridUnit* Target = nullptr;
	UPROPERTY()
	TMap<FGameplayTag, int32> TargetAttributes;
	UPROPERTY()
	TArray<FGameplayTag> TargetTraits;

	void CalculateCombat(UGridManager* InGridManager, AGridUnit* InstigatorUnit, AGridUnit* TargetUnit);


	// JUST TRY THIS FOR NOW
	int32 CalculateAttackSpeed(AGridUnit* Unit) const;
	int32 CalculateHitRate(AGridUnit* Unit) const;
	int32 CalculateAvoid(AGridUnit* Unit) const;
};

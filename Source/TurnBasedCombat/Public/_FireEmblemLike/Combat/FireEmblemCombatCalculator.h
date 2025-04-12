// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireEmblemCombatStructs.h"
#include "UObject/Object.h"
#include "FireEmblemCombatCalculator.generated.h"

class AFireEmblemUnit;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UFireEmblemCombatCalculator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void CalculateAllCombatPredictions(FFireEmblemCombatPrediction& OutCombatPrediction, AFireEmblemUnit* ActiveUnit);

	void GetCombatPredictionFromInformation(
		FFireEmblemCombatPrediction& OutCombatPrediction, const FFireEmblemCombatInformation& CombatInformation) const;
	
	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	void GetCombatOutcome(
		FFireEmblemCombatOutcome& Outcome, AFireEmblemUnit* InstigatorUnit, const FName WeaponName, AFireEmblemUnit* TargetUnit) const;

	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	TArray<AFireEmblemUnit*> GetCombatOrder(FFireEmblemCombatOutcome& Outcome);
	
	// unit
	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	void GetUnitSnapshotBasic(FFireEmblemStatsBasic& OutSnapshot, AFireEmblemUnit* InGridUnit, const FName WeaponName) const;
	UFUNCTION(BlueprintCallable, Category="Combat Calculator")
	void GetUnitSnapshotAdvanced(FFireEmblemStatsAdvanced& OutSnapshot, const FFireEmblemStatsBasic& InstigatorSnapshot, const FFireEmblemStatsBasic& TargetSnapshot) const;

	// Weapon ~ start
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetWeapon(FWeaponTraits& WeaponTraits, const FGameplayTag& WeaponTag) const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	int32 GetWeaponRange(const FGameplayTag& WeaponTag) const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UWeaponDataAsset* GetWeaponDataAsset();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetWeaponByName(FFireEmblemWeaponContainer& WeaponContainer, const FName& WeaponName) const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetWeaponTraitsByName(FWeaponTraits& WeaponTraits, const FName& WeaponName) const;

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetWeaponRangesByName(const TArray<FName>& WeaponNames) const;
	// Weapon ~ end
	
protected:
	static int32 GetMaxDamage() { return 60; }
	static int32 CalculateAttackSpeed(const AFireEmblemUnit* Unit, FFireEmblemWeaponTraits& WeaponTraits);
	static int32 CalculateHitRate(const AFireEmblemUnit* Unit, FFireEmblemWeaponTraits& WeaponTraits);
	static int32 CalculateAvoid(const FFireEmblemStatsBasic& Snapshot, const AFireEmblemUnit* Unit, const AGridTile* Tile);
	static int32 CalculateCriticalRate(const AFireEmblemUnit* Unit, FFireEmblemWeaponTraits& WeaponTraits);
	static int32 CalculateCriticalAvoid(const AFireEmblemUnit* Unit, const AGridTile* Tile);

	// AI...
public:
	void CalculateCombatScore(FFireEmblemCombatScore& CombatScore) const;
	
protected:
	// Evaluations ~ start
	// QUESTIONS
	// we need to provide a number of metrics by which a unit class/unit status can multiply its weights against
	// ideally all these questions will become an interface that another class can implement that will be assigned to a unit in editor
	// all questions should return a value between 0-1
	
	UFUNCTION(BlueprintCallable)
	void GetCombatEvaluation(FFireEmblemCombatEvaluation& OutCombatEvaluation, const FFireEmblemCombatPrediction& InCombatPrediction);
	
	float HitChanceInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float HitChanceTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float CriticalChanceInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float CriticalChanceTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float HealthChangePercentageInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float HealthChangePercentageTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float DefeatedInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	float DefeatedTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const;
	// Evaluations ~ end
	
};
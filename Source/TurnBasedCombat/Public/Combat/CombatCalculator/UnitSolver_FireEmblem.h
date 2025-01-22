// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitSolver.h"
#include "UnitSolver_FireEmblem.generated.h"

struct FCombatCalculatorPayload_FireEmblem;
// struct FCombatCalculatorPayload_FireEmblem;
/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UUnitSolver_FireEmblem : public UUnitSolver
{
	GENERATED_BODY()
	
public:
	UUnitSolver_FireEmblem();
	
	virtual void GetUnitSolved(TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits) override;
	virtual void Solve(
		TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
		const FCombatCalculatorPayload_FireEmblem& Payload);
	virtual void Solve(
		TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
		const UGridManager* GridManager, AGridUnit* Instigator, AGridUnit* Target);
	virtual int32 SolveAttribute(
		FGameplayTag Attribute, TArray<FGameplayTag> Traits,
		const UGridManager* GridManager, AGridUnit* Instigator, AGridUnit* Target);
	
	
	// void SetGridManager(UGridManager* InGridManager) { GridManager = InGridManager; }
	// void SetInstigator(AGridUnit* InInstigator) { Instigator = InInstigator; }
	// void SetTarget(AGridUnit* InTarget) { Target = InTarget; }

protected:
	// UPROPERTY()
	// UGridManager* GridManager;
	// UPROPERTY()
	// AGridUnit* Instigator;
	// UPROPERTY()
	// AGridUnit* Target;
};

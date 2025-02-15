// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatWorldSubsystem.generated.h"


class UGameplayAbility;
class UCombatCalculator_Basic;
class UActionEvaluator_Combat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatWorldSubsystemEventDelegate, const FCombatPrediction&, CombatPrediction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatWorldSubsystemDelegate, const AGridUnit*, Instigator, const AGridUnit*, Target);

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UCombatWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void PostInitialize() override;
	
public:
	// FOR NOW... just use this as an event focale point
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FCombatWorldSubsystemDelegate OnCombatStart;
	
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FCombatWorldSubsystemEventDelegate OnCombatEnd;

	UFUNCTION()
	void InitiateCombat(FCombatPrediction InCombatPrediction);
	
protected:
	UPROPERTY()
	FCombatPrediction CombatPrediction;
	
	UPROPERTY()
	const UCombatCalculator_Basic* CombatCalculator = nullptr;

	UPROPERTY()
	AGridUnit* ActiveUnit = nullptr;

	UFUNCTION()
	void DoCombatTurn();
	
	UFUNCTION()
	void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);
	
	FDelegateHandle DelegateHandle;

	// AI ~ start
	UPROPERTY()
	UActionEvaluator_Combat* CombatEvaluator = nullptr;

	UPROPERTY()
	TArray<AGridUnit*> UnitsToExecuteTurns;
	UFUNCTION()
	void ExecuteEnemyTurn(FGameplayTag FactionTag);
	UFUNCTION()
	void InitiateUnitCombat(const FCombatPrediction& InCombatPrediction);
	
	// AI ~ end
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatWorldSubsystem.generated.h"


class UCombatCalculator_Basic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatWorldSubsystemDelegate, const AGridUnit*, Instigator, const AGridUnit*, Target);

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UCombatWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// FOR NOW... just use this as an event focale point
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FCombatWorldSubsystemDelegate OnCombatStart;
	
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FCombatWorldSubsystemDelegate OnCombatEnd;

	UFUNCTION()
	void InitiateCombat(AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit);
	
protected:
	UPROPERTY()
	const UCombatCalculator_Basic* CombatCalculator = nullptr;

	UPROPERTY()
	FCombatSnapshot_Outcome CombatOutcome;
	
	UPROPERTY()
	AGridUnit* InstigatorUnit = nullptr;

	UPROPERTY()
	AGridUnit* TargetUnit = nullptr;

	UPROPERTY()
	AGridUnit* ActiveUnit = nullptr;
		
	UFUNCTION()
	void DoCombatTurn();
	
	UFUNCTION()
	void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);
	
	FDelegateHandle DelegateHandle;
};

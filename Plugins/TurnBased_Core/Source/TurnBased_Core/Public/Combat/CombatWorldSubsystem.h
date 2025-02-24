// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatWorldSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_Combat, Log, All);

class UGameplayAbility;
class UCombatCalculator_Basic;
class UActionEvaluator_Combat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatWorldSubsystemEventDelegate, const FCombatPrediction&, CombatPrediction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatWorldSubsystemDelegate, AGridUnit*, Instigator, AGridUnit*, Target);

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

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsCombatActive() const { return bCombatActive; }

protected:
	UPROPERTY()
	bool bCombatActive = false;

	UPROPERTY()
	int32 CombatOrderIndex = 0;
	
	UPROPERTY()
	FCombatPrediction CombatPrediction;	

public:
	UFUNCTION()
	void InitiateCombat(const FCombatPrediction& InCombatPrediction);

protected:
	UFUNCTION()
	void SendCombatEventToNextUnit();
	
	UFUNCTION()
	void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);

private:
	UPROPERTY()
	const UCombatCalculator_Basic* CombatCalculator = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "UObject/Object.h"
#include "CombatFlowHelper.generated.h"


class UGameEventTaskManager;
class AGridUnit;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatFlowComplete);

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UCombatFlowHelper : public UObject
{
	GENERATED_BODY()

	UCombatFlowHelper();
	
public:
	static UCombatFlowHelper* Create(
		const FCombatPrediction& Prediction, UGameEventTaskManager* InPreAttackEventManager);

	FOnCombatFlowComplete OnCombatFlowComplete;

	UFUNCTION()
	void Start();
	
	// UPROPERTY()
	// AGridUnit* Instigator = nullptr;
	//
	// UPROPERTY()
	// AGridUnit* Target = nullptr;

protected:
	UPROPERTY()
	FCombatPrediction CombatPrediction;

	UPROPERTY()
	UGameEventTaskManager* PreAttackEvent = nullptr;

	UFUNCTION()
	void OnInstigatorAbilityComplete(const FAbilityEndedData& AbilityEndedData);

private:
	bool bInstigatorComplete = false;
	bool bTargetComplete = false;
	
	// UPROPERTY()
	// UGameEventTaskManager* PostAttackEvent = nullptr;
	//
	// UPROPERTY()
	// UGameEventTaskManager* PreReactionEvent = nullptr;
	//
	// UPROPERTY()
	// UGameEventTaskManager* PostReactionEvent = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/CombatData.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIWorldSubsystem.generated.h"


class UActionEvaluator_Combat;
class AGridUnit;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UAIWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void PostInitialize() override;

	UPROPERTY()
	bool bTurnActive = false;
	
	UFUNCTION()
	void StartTurn(FGameplayTag FactionTag);

	void EndTurn(FGameplayTag FactionTag);

	UFUNCTION()
	void GetCombatPrediction();

	UFUNCTION()
	void OnCombatEnd(const FCombatPrediction& InCombatPrediction);
	
	UPROPERTY()
	FGameplayTag ActiveFaction;
	
	UPROPERTY()
	TArray<AGridUnit*> UnitsToExecuteTurns;

	UPROPERTY()
	int32 UnitsToExecuteTurnsIndex = 0;
	
	UPROPERTY()
	UActionEvaluator_Combat* CombatEvaluator = nullptr;

	UPROPERTY()
	FCombatPrediction CombatPrediction;
};

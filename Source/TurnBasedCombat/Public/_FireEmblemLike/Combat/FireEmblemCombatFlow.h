// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireEmblemCombatStructs.h"
#include "UObject/Object.h"
#include "FireEmblemCombatFlow.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FFireEmblemCombatFlowSignature, const FFireEmblemCombatPrediction&, CombatPrediction);


/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UFireEmblemCombatFlow : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Combat Flow")
	FFireEmblemCombatFlowSignature OnCombatStartDelegate;
	
	UPROPERTY(EditAnywhere, Category="Combat Flow")
	FFireEmblemCombatFlowSignature OnCombatEndDelegate;

	UFUNCTION(BlueprintCallable, Category="Combat Flow")
	bool IsCombatActive() const { return bCombatActive; }

	UFUNCTION()
	void InitiateCombat(const FFireEmblemCombatPrediction& InCombatPrediction);
	
protected:
	UPROPERTY()
	bool bCombatActive = false;

	UPROPERTY()
	int32 CombatOrderIndex = 0;
	
	UPROPERTY()
	FFireEmblemCombatPrediction CombatPrediction;

protected:
	UFUNCTION()
	void SendCombatEventToNextUnit();

	UPROPERTY()
	AFireEmblemUnit* ActiveUnit = nullptr;
	
	UPROPERTY()
	UGameplayAbility* ActiveGameplayAbility = nullptr;
	
	UFUNCTION()
	void CacheGameplayAbility(UGameplayAbility* InGameplayAbility);
	
	UFUNCTION()
	void CheckGameplayAbility(UGameplayAbility* InGameplayAbility);

};

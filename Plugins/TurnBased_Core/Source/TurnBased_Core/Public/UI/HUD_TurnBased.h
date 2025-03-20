// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/CombatData.h"
#include "GameFramework/HUD.h"
#include "HUD_TurnBased.generated.h"


class AGridUnit;
class UUserWidget_TurnBased;
class UUserWidget_ActionOptions;
class UCombatCalculator_Basic;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActivateWidgetDelegate, UUserWidget_TurnBased*, Widget, bool, RequiresCursor);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASED_CORE_API AHUD_TurnBased : public AHUD
{
	GENERATED_BODY()

public:
	AHUD_TurnBased();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FActivateWidgetDelegate OnWidgetActivated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FActivateWidgetDelegate OnWidgetDeactivated;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_TurnBased* ActivateWinLevelWidget();

	// Options ~ start
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_ActionOptions* ActivateActionOptionsWidget(AGridUnit* TargetUnit, const TArray<FGameplayTag>& ActionTags);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_ActionOptions* RemoveActionOptionsWidget();
	// Options ~ end

	// Combat ~ start
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_TurnBased* ActivateCombatPredictionWidget(const FCombatPrediction& InCombatPrediction);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateCombatPredictionWidget(const FCombatPrediction& InCombatPrediction);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_TurnBased* RemoveCombatPredictionWidget();
	// Combat ~ end
};

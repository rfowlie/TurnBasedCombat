// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "Unit/GridUnit.h"
#include "HUD_TurnBased.generated.h"


class AGridUnit;
class UUserWidget_TurnBased;
class UUserWidget_ActionOptions;
class UCombatCalculator_Basic;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASED_CORE_API AHUD_TurnBased : public AHUD
{
	GENERATED_BODY()

public:
	AHUD_TurnBased();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_ActionOptions* ActivateActionOptionsWidget(AGridUnit* TargetUnit, const TArray<FGameplayTag>& ActionTags);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_ActionOptions* RemoveActionOptionsWidget();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UUserWidget_TurnBased* ActivateCombatPredictionWidget(AGridUnit* InInstigator, AGridUnit* InTarget);
	
protected:
	// FOR NOW
	// things will be a lot smoother if HUD has a copy of the combat calculator...
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	UCombatCalculator_Basic* CombatCalculator = nullptr;
	
};

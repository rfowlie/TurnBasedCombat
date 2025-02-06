// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "HUD_TurnBased.generated.h"


class AGridUnit;
class UWidget_ActionOptions;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API AHUD_TurnBased : public AHUD
{
	GENERATED_BODY()

public:	
	// UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	// UWidget_ActionOptions* ActivateActionOptions(AGridUnit* TargetUnit, TArray<FGameplayTag> ActionTags);
	
	UFUNCTION(BlueprintImplementableEvent)
	UWidget_ActionOptions* ActivateAction(AGridUnit* TargetUnit, TArray<FGameplayTag> ActionTags);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UWidget_ActionOptions> ActionOptionsClass;
	
	UPROPERTY(BlueprintReadOnly, Category = UI)
	UWidget_ActionOptions* ActionOptionInstance = nullptr;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UserWidget_TurnBased.h"
#include "UserWidget_ActionOptions.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetActionSelectedDelegate, FGameplayTag, ActionTag);

/**
 * layout all the desired actions that we want from this widget...s
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API UUserWidget_ActionOptions : public UUserWidget_TurnBased
{
	GENERATED_BODY()

public:
	// actions
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetActionSelectedDelegate OnActionSelected;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ActionOptions.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetActionOptionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetActionSelectedDelegate, FGameplayTag, ActionTag);

/**
 * layout all the desired actions that we want from this widget...s
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API UWidget_ActionOptions : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FWidgetActionOptionDelegate OnInitiate;
	
	UPROPERTY(BlueprintAssignable)
	FWidgetActionOptionDelegate OnComplete;

	// actions
	UPROPERTY(BlueprintAssignable)
	FWidgetActionSelectedDelegate OnActionSelected;
	
	// UPROPERTY(BlueprintAssignable)
	// FWidgetActionOptionDelegate OnAction_Attack;
	//
	// UPROPERTY(BlueprintAssignable)
	// FWidgetActionOptionDelegate OnAction_Skill;
	//
	// UPROPERTY(BlueprintAssignable)
	// FWidgetActionOptionDelegate OnAction_Item;
};

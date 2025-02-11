// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget_TurnBased.h"
#include "UserWidget_HandleAnimation.generated.h"

/**
 * create functions and callbacks to trigger the widgets animation
 */
UCLASS()
class TURNBASED_CORE_API UUserWidget_HandleAnimation : public UUserWidget_TurnBased
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetActionOptionDelegate OnAnimationBegin;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetActionOptionDelegate OnAnimationComplete;
};

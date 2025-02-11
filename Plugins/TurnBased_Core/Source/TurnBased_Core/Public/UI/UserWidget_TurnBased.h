// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_TurnBased.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetActionOptionDelegate);

/**
 * this will act as a base widget class for this plugin
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASED_CORE_API UUserWidget_TurnBased : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetActionOptionDelegate OnBegin;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetActionOptionDelegate OnComplete;
};

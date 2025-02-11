// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "ControllerState_EndTurn.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_EndTurn : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_EndTurn();

public:
	static UControllerState_EndTurn* Create();

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
};

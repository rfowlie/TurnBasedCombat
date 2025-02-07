// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "ControllerState_Combat.generated.h"

/**
 * this is a watching state that will not accept any input and disable the mouse?
 * but will listen for a callback to indicate that combat is finished, then pass in the idle state
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_Combat : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_Combat();
	
	public:
	static UControllerState_Combat* Create();	
	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	
};

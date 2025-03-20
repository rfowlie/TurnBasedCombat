// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "ControllerState_Wait.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_Wait : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_Wait();
	
public:
	static UControllerState_Wait* Create();
	
	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;

protected:
	UFUNCTION()
	void OnCombatStart(AGridUnit* Instigator, AGridUnit* Target);
};

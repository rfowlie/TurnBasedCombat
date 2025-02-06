// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "ControllerState_Idle.generated.h"

class AGridUnit;
class UInputAction;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_Idle : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_Idle();

public:
	static UControllerState_Idle* Create();
	
protected:
	virtual UInputMappingContext* CreateInputMappingContext() override;
	
	UPROPERTY()
	UInputAction* InputAction_Select = nullptr;
	virtual void OnSelect();
};

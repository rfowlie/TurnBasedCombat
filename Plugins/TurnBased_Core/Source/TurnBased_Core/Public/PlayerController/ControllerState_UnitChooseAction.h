// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "ControllerState_UnitChooseAction.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_UnitChooseAction : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_UnitChooseAction();

public:
	static UControllerState_UnitChooseAction* Create();
};

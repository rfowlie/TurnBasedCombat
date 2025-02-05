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

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	virtual void OnExit(const APlayerController* InPlayerController) override;
	
protected:
	virtual UInputMappingContext* CreateInputMappingContext(APlayerController* PlayerController) override;
	
	UPROPERTY()
	UInputAction* InputAction_Select = nullptr;
	virtual void OnSelect();

	UFUNCTION()
	void OnUnitSelected(AGridUnit* InGridUnit) const;
};

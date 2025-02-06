// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "ControllerState_UnitAttack.generated.h"


class AGridUnit;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_UnitAttack : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_UnitAttack();
	
public:
	static UControllerState_UnitAttack* Create(AGridUnit* InActiveUnit);

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	virtual void OnExit() override;
	
protected:
	UPROPERTY()
	AGridUnit* ActiveUnit;
};

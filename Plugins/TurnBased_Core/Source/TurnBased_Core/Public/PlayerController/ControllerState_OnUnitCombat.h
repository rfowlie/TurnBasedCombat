// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "ControllerState_OnUnitCombat.generated.h"

class UGameplayAbility;
class AGridUnit;
/**
 * this is a watching state that will not accept any input and disable the mouse?
 * but will listen for a callback to indicate that combat is finished, then pass in the idle state
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_OnUnitCombat : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_OnUnitCombat();
	
	public:
	static UControllerState_OnUnitCombat* Create(AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit);	
	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	virtual void OnExit() override;

protected:
	UPROPERTY()
	AGridUnit* InstigatorUnit = nullptr;

	UPROPERTY()
	AGridUnit* TargetUnit = nullptr;

	UFUNCTION()
	void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);
	
	FDelegateHandle DelegateHandle;
};

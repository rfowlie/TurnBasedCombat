// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "ControllerState_OnUnitMove.generated.h"

class UGameplayAbility;
class AGridUnit;

/**
 * call GA move on unit, bind to GA finished callback, prevent player input
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_OnUnitMove : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_OnUnitMove();
	
public:
	static UControllerState_OnUnitMove* Create(AGridUnit* InActiveUnit, AGridTileBase* InTargetTile);
	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	virtual void OnExit() override;

protected:
	UPROPERTY()
	AGridUnit* ActiveUnit = nullptr;

	UPROPERTY()
	AGridTile* TargetTile = nullptr;

	UFUNCTION()
	void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);

private:
	FDelegateHandle GameplayAbilityDelegateHandle;
};

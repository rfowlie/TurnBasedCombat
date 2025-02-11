// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBased_Core_Tags.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "Tile/GridTile.h"
#include "ControllerState_UnitChooseAction.generated.h"


class UInputAction;
class AGridUnit;
class UUserWidget_ActionOptions;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_UnitChooseAction : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_UnitChooseAction();

public:
	static UControllerState_UnitChooseAction* Create(
		AGridUnit* InActiveUnit, AGridTile* InTargetTile);

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	
	virtual void OnExit() override;

protected:
	UPROPERTY()
	AGridUnit* ActiveUnit = nullptr;

	UPROPERTY()
	AGridTile* TargetTile = nullptr;
	
	UPROPERTY()
	UUserWidget_ActionOptions* Widget_ActionOptions = nullptr;

	UFUNCTION()
	void OnActionSelected(FGameplayTag ActionTag);

	virtual UInputMappingContext* CreateInputMappingContext() override;

	UPROPERTY()
	UInputAction* InputAction_Deselect = nullptr;
	UFUNCTION()
	void OnDeselect();
	
};

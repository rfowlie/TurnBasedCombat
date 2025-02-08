// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "InputActionValue.h"
#include "UI/UserWidget_TurnBased.h"
#include "ControllerState_Attack_TileSelected.generated.h"

class UInputAction;
class AGridUnit;

/**
 * PRE: target selected
 * cache previous unit location, set new location
 * potentially change the state of the tile?
 * open the combat UI, make sure to only showing weapons that are valid
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_Attack_TileSelected : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_Attack_TileSelected();

public:
	static UControllerState_Attack_TileSelected* Create(
		AGridUnit* InstigatorUnit, AGridUnit* TargetUnit, AGridTile* TileSelected, const TMap<AGridTile*, int32>& InAttackTileRangeMap);

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	virtual void OnExit() override;
	
protected:
	virtual UInputMappingContext* CreateInputMappingContext() override;

	UPROPERTY()
	UInputAction* InputAction_Select = nullptr;
	UFUNCTION()
	void OnSelect();
	
	UPROPERTY()
	UInputAction* InputAction_Deselect = nullptr;
	UFUNCTION()
	void OnDeselect();

	UPROPERTY()
	UInputAction* InputAction_CycleWeapon = nullptr;
	UFUNCTION()
	void OnCycleTile(const FInputActionValue& Value);
	
	
	UPROPERTY()
	AGridUnit* InstigatorUnit = nullptr;
	
	UPROPERTY()
	AGridUnit* TargetUnit = nullptr;
	
	UPROPERTY()
	AGridTile* TileSelected = nullptr;

	UFUNCTION()
	void CombatInitiated();

private:
	UPROPERTY()
	AGridTile* InstigatorInitialTile = nullptr;
	UPROPERTY()
	FRotator InstigatorInitialRotation = FRotator();

	UPROPERTY()
	TMap<AGridTile*, int32> AttackTileRangeMap;
	
	UPROPERTY()
	TArray<AGridTile*> AttackTileOrder;

	UPROPERTY()
	UUserWidget_TurnBased* Widget;

};

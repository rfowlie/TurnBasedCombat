// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "Grid/GridStructs.h"
#include "ControllerState_UnitSelected.generated.h"

class UInputAction;
class UInputMappingContext;
class AGridTile;
class AGridUnit;


/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_UnitSelected : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_UnitSelected();
	
public:
	static UControllerState_UnitSelected* Create(AGridUnit* InSelectedUnit, const int32 InAvailableMovement);

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	virtual void OnExit() override;

protected:
	virtual UInputMappingContext* CreateInputMappingContext() override;

	UPROPERTY()
	UInputAction* InputAction_Select = nullptr;
	virtual void OnSelect();
	
	UPROPERTY()
	UInputAction* InputAction_Deselect = nullptr;
	virtual void OnDeselect();
	
	UPROPERTY()
	UInputAction* InputAction_CycleUnit = nullptr;
	virtual void OnCycleUnit();		

	UPROPERTY()
	bool IsPlayerUnit = false;

	UPROPERTY()
	int32 AvailableMovement = 0;
	
	UPROPERTY()
	AGridUnit* ActiveUnit = nullptr;

	UPROPERTY()
	AGridTile* SelectedUnitTile = nullptr;

	UPROPERTY()
	TArray<FGridMovement> GridMovements;

	UPROPERTY()
	TArray<AGridUnit*> EnemyUnitsInRange;

	UFUNCTION()
	bool SetMovementTiles();
	
	UFUNCTION()
	void MoveSelectedTarget(AGridTile* InGridTile);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Abstract.h"
#include "Grid/GridStructs.h"
#include "Unit/GridUnit.h"
#include "ControllerState_Attack_TargetSelected.generated.h"


class UInputAction;
struct FGridMovement;
class AGridUnit;

/**
 * PRE: unit select?
 * SITUATION: the player has selected a unit to attack
 * show all the tiles from which active unit can attack target unit from
 * these tiles should coincide with the available movement tiles
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_Attack_TargetSelected : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_Attack_TargetSelected();
	
public:
	static UControllerState_Attack_TargetSelected* Create(
		AGridUnit* InActiveUnit, AGridUnit* InTargetUnit, const TArray<FGridMovement>& GridMovements);

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
	AGridUnit* InstigatorUnit = nullptr;
	
	UPROPERTY()
	AGridUnit* TargetUnit = nullptr;

	UPROPERTY()
	TArray<FGridMovement> GridMovements;

	UPROPERTY()
	TMap<AGridTile*, int32> AttackTileRangeMap;
	
};

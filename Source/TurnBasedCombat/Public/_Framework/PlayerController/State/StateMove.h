﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractPlayerControllerState.h"
#include "Grid/Unit/GridUnit.h"
#include "StateMove.generated.h"


class UGameplayAbility;
struct FGridMovement;
class UGridProxy;
class UGridManager;
class UInputAction;
class AGridTile;
class AGridUnit;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UStateMove : public UAbstractPlayerControllerState
{
	GENERATED_BODY()

public:
	UStateMove();
	
	void Initialize(UGridManager* InGridManager);
	
	FGameplayAbilityEndCallback ResetCallback;
	UFUNCTION()
	void Reset(UGameplayAbility* GameplayAbility);
	

protected:
	enum class EMovePhase
	{
		None,
		Idle,
		SelectedMoveUnit,
		SelectedMoveUnitNotTeam,
		SelectedMoveTile
	};
	
	EMovePhase Phase = EMovePhase::Idle;
	
	UPROPERTY()
	UGridManager* GridManager = nullptr;
	UPROPERTY()
	UGridProxy* GridProxyCurrent = nullptr;
	UPROPERTY()
	UGridProxy* GridProxyMoveTo = nullptr;
	
	UPROPERTY()
	UInputAction* IA_Select = nullptr;
	UPROPERTY()
	UInputAction* IA_Deselect = nullptr;
	UPROPERTY()
	UInputAction* IA_Cycle = nullptr;
	
	virtual UInputMappingContext* SetupInputMappingContext(APlayerController* PlayerController) override;

	virtual void OnEnter() override;
	virtual void OnExit() override;
	
	virtual void OnSelect();
	virtual void OnDeselect();
	virtual void OnCycleUnit();

public:
	UFUNCTION()
	void Enable();
	UFUNCTION()
	void Disable();
};

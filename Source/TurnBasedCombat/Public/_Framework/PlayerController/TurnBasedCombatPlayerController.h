// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TurnBasedCombatPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class UAbstractPlayerControllerState;
class UGridManager;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API ATurnBasedCombatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATurnBasedCombatPlayerController();
	virtual void BeginPlay() override;

	virtual void Initialize(UGridManager* InGridManager);

protected:
	UPROPERTY()
	UGridManager* GridManager = nullptr;
	
	UPROPERTY()
	TArray<UAbstractPlayerControllerState*> ControllerStates;
	UPROPERTY()
	UAbstractPlayerControllerState* ControllerStateCurrent = nullptr;
	UPROPERTY()
	int32 ControllerStateIndex = -1;
	UFUNCTION()
	void SetState(UAbstractPlayerControllerState* NewState);

	void CreateDefaultInputMapping();
	UFUNCTION()
	void AddInputMapping();
	UFUNCTION()
	void RemoveInputMapping();

	UPROPERTY()
	UInputMappingContext* InputMappingContext = nullptr;
	UPROPERTY()
	UInputAction* IA_Pause = nullptr;
	UFUNCTION()
	void PauseMenu();
	UPROPERTY()
	UInputAction* IA_CycleMode = nullptr;
	UFUNCTION()
	void CycleMode();
	UPROPERTY()
	UInputAction* IA_MoveCamera = nullptr;
	UFUNCTION()
	void Controls();
};

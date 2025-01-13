// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "GameFramework/PlayerController.h"
#include "TurnBasedCombatPlayerController.generated.h"


class AGridTile;
class UStateAttack;
class UStateMove;
class UCombatDisplayWidget;
class UInputMappingContext;
class UInputAction;
class UAbstractPlayerControllerState;
class UGridManager;


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Combat_GridTileInfo);

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
	UAbstractPlayerControllerState* StateWait = nullptr;
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

	void CreateStates();
	UFUNCTION()
	void EnableState();
	UFUNCTION()
	void DisableState();
	
private:
	// FOR NOW
	UPROPERTY()
	UStateMove* StateMove = nullptr;
	UPROPERTY()
	UStateAttack* StateAttack = nullptr;
};

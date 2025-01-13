// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AbstractPlayerControllerState.h"
#include "StateAttack.generated.h"


class ATurnBasedCombatHUD;
class UCombatDisplayWidget;
struct FCombatDisplayInfo;

// mode
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack);
// phase
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack_None);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack_Idle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack_SelectedAttacker);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack_SelectedTarget);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack_SelectedAttackTile);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack_Combat);

// tile
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Tile_State_Attacker);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Tile_State_CanTargetFrom);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Tile_State_Target);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Tile_State_TargetFrom);

// UI
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Combat_DuelInfo);


class UInputAction;
class UGridProxy;
class UGridManager;


/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UStateAttack : public UAbstractPlayerControllerState
{
	GENERATED_BODY()

	UPROPERTY()
	ATurnBasedCombatHUD* HUD = nullptr;
	
public:
	UStateAttack();
	void Initialize(UGridManager* InGridManager, AHUD* InTurnBasedCombatHUD);

	virtual FGameplayTag GetStateTag() const override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FStateAttackDelegate, const FCombatDisplayInfo&)
	FStateAttackDelegate OnPotentialAttack;
	
protected:
	enum class EAttackPhase
	{
		None,
		Idle,
		SelectedAttacker,
		SelectedTarget,
		SelectedAttackTile
	};
	
	// EAttackPhase Phase;
	FGameplayTag TagPhase;
	void SetPhase(FGameplayTag InTagPhase);

	UPROPERTY()
	UGridManager* GridManager = nullptr;
	UPROPERTY()
	UGridProxy* Instigator = nullptr;
	UPROPERTY()
	UGridProxy* Target = nullptr;
	UPROPERTY()
	UGridProxy* MovingTo = nullptr;

	UPROPERTY()
	UInputAction* IA_Select = nullptr;
	UPROPERTY()
	UInputAction* IA_Deselect = nullptr;
	UPROPERTY()
	UInputAction* IA_Cycle = nullptr;
	
	virtual UInputMappingContext* SetupInputMappingContext(
		APlayerController* PlayerController) override;

	virtual void OnEnter() override;
	virtual void OnExit() override;
	
	virtual void OnSelect();
	virtual void OnDeselect();	
	virtual void OnCycleUnit();
	
	void UndoAll();
	void UndoSelectedAttacker();
	void UndoSelectedTarget();
	void UndoSelectedAttackTile();
	
	UFUNCTION()
	void Enable();
	UFUNCTION()
	void Disable();
	
};

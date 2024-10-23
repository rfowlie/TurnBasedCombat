// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AbstractPlayerControllerState.h"
#include "StateAttack.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Attacker);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Attackable);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Target);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_TargetFrom);


class UInputAction;
class UGridProxy;
class UGridManager;

// USTRUCT()
// struct FTilesInRange
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	TMap<AATBGridTile*, int32> Mappings;
// };

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UStateAttack : public UAbstractPlayerControllerState
{
	GENERATED_BODY()

public:
	UStateAttack();
	void Initialize(UGridManager* InGridManager);

protected:
	enum class EAttackPhase
	{
		None,
		Idle,
		SelectedAttacker,
		SelectedTarget,
		SelectedAttackTile
	};
	
	EAttackPhase Phase = EAttackPhase::Idle;
	
	UPROPERTY()
	UGridManager* GridManager = nullptr;
	UPROPERTY()
	UGridProxy* GridProxyCurrent = nullptr;
	UPROPERTY()
	UGridProxy* GridProxyTarget = nullptr;
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
	
	void UndoAll();
	void UndoSelectedAttacker();
	void UndoSelectedTarget();
	void UndoSelectedAttackTile();
};

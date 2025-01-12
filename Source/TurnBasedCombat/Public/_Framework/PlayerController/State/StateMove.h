// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractPlayerControllerState.h"
#include "StateMove.generated.h"


class UGameplayAbility;
struct FGridMovement;
class UGridProxy;
class UGridManager;
class UInputAction;
class AGridTile;
class AGridUnit;


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Move);

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UStateMove : public UAbstractPlayerControllerState
{
	GENERATED_BODY()

public:
	UStateMove();

	virtual FGameplayTag GetStateTag() const override;
	
	void Initialize(UGridManager* InGridManager);
	

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

// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateAttack.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EventSystem/Events/EventUnitAttack.h"
#include "EventSystem/Events/EventUnitMove.h"
#include "Grid/GridManager.h"
#include "Grid/GridProxy.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack, "Encounter.Mode.Attack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Attacker, "Grid.State.Attacker");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Attackable, "Grid.State.Attackable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Target, "Grid.State.Target");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_TargetFrom, "Grid.State.TargetFrom");


UStateAttack::UStateAttack()
{
}

void UStateAttack::Initialize(UGridManager* InGridManager)
{
	// only allow setting once?
	if (GridManager == nullptr && InGridManager)
	{
		GridManager = InGridManager;
	}
}

UInputMappingContext* UStateAttack::SetupInputMappingContext(APlayerController* PlayerController)
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);
	
	IA_Deselect = NewObject<UInputAction>(this);
	IA_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(IA_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(IA_Deselect, ETriggerEvent::Started, this, &UStateAttack::OnDeselect);
	
	IA_Cycle = NewObject<UInputAction>(this);
	IA_Cycle->ValueType = EInputActionValueType::Axis1D;
	FEnhancedActionKeyMapping& Mapping_Cycle_Right = NewInputMappingContext->MapKey(IA_Cycle, EKeys::E);
	FEnhancedActionKeyMapping& Mapping_Cycle_Left = NewInputMappingContext->MapKey(IA_Cycle, EKeys::Q);
	Mapping_Cycle_Left.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	EIC->BindAction(IA_Cycle, ETriggerEvent::Started, this, &UStateAttack::OnCycleUnit);
	
	return NewInputMappingContext;
}

void UStateAttack::OnEnter()
{	
	UE_LOG(LogTemp, Warning, TEXT("OnEnter - ATTACK"));
	
	Phase = EAttackPhase::Idle;	
}

void UStateAttack::OnExit()
{
	Super::OnExit();

	UndoAll();
}

void UStateAttack::OnSelect()
{
	UGridProxy* GridProxy = GridManager->GetCurrentHoveredGridTile();
	if (!GridProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridProxy is null"));
		return;
	}

	switch(Phase)
	{
	case EAttackPhase::None:
		break;
	case EAttackPhase::Idle:
		// check if previous selection, and undo changes
		if (GridProxyCurrent)
		{
			GridProxyCurrent->UndoAll();
		}
		// set new selection
		GridProxyCurrent = GridProxy;
		GridProxyCurrent->SetTargetableEnemies(true);
		// if player unit selected move to next phase
		if (GridProxyCurrent->IsPlayer())
		{
			Phase = EAttackPhase::SelectedAttacker;
		}
		break;
	case EAttackPhase::SelectedAttacker:
		// check if new player unit selected
		if (GridProxyCurrent != GridProxy && GridProxy->IsPlayer())
		{
			GridProxyCurrent->UndoAll();
			GridProxyCurrent = GridProxy;
			GridProxyCurrent->SetTargetableEnemies(true);
			Phase = EAttackPhase::SelectedAttacker;
		}
		// check if new enemy target selected
		if (GridProxyTarget != GridProxy && GridProxy->IsEnemy())
		{
			GridProxyTarget->UndoAll();
			GridProxyTarget = GridProxy;
			GridProxyCurrent->SetAttackTiles(GridProxyTarget, true);
			Phase = EAttackPhase::SelectedTarget;
		}
		break;
	case EAttackPhase::SelectedTarget:
		// check if new player unit selected
		if (GridProxyCurrent != GridProxy && GridProxy->IsPlayer())
		{
			GridProxyCurrent->UndoAll();
			GridProxyCurrent = GridProxy;
			GridProxyCurrent->SetTargetableEnemies(true);
			Phase = EAttackPhase::SelectedAttacker;
		}
		// check if new enemy target selected
		else if (GridProxyTarget != GridProxy && GridProxy->IsEnemy())
		{
			GridProxyTarget->UndoAll();
			GridProxyTarget = GridProxy;
			GridProxyCurrent->SetAttackTiles(GridProxyTarget, true);
			Phase = EAttackPhase::SelectedTarget;
		}
		// check if tile can attack from is selected
		else if (GridProxyTarget->IsAttackTile(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			Phase = EAttackPhase::SelectedAttackTile;
		}
		break;
	case EAttackPhase::SelectedAttackTile:
		// check if other tile can attack from is selected
		if (GridProxyMoveTo != GridProxy && GridProxyTarget->IsAttackTile(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			Phase = EAttackPhase::SelectedAttackTile;
		}
		// check if same tile is selected to confirm attack
		if (GridProxyMoveTo == GridProxy)
		{
			TArray<UAbstractEvent*> Events;
			UEventUnitMove* EventMove = NewObject<UEventUnitMove>(this);
			UEventUnitAttack* EventAttack = NewObject<UEventUnitAttack>(this);
			Events.AddUnique(EventMove);
			Events.AddUnique(EventAttack);
			OnEventCreate.Execute(Events);
		}
		break;
	}
}

void UStateAttack::OnDeselect()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeselect - ATTACK"));
	
	switch (Phase) {
	case EAttackPhase::None:
		break;
	case EAttackPhase::Idle:
		break;
	case EAttackPhase::SelectedAttacker:
		UndoSelectedAttacker();
		Phase = EAttackPhase::Idle;
		break;
	case EAttackPhase::SelectedTarget:
		UndoSelectedTarget();
		Phase = EAttackPhase::SelectedAttacker;
		break;
	case EAttackPhase::SelectedAttackTile:
		UndoSelectedAttackTile();
		Phase = EAttackPhase::SelectedTarget;
		break;
	}
}

void UStateAttack::OnCycleUnit()
{
	UndoAll();
	
	GridProxyCurrent = GridManager->GetNextGridUnit(GridProxyCurrent);
	GridProxyCurrent->SetTargetableEnemies(true);
	Phase = EAttackPhase::SelectedAttacker;
}

void UStateAttack::UndoAll()
{
	UndoSelectedAttackTile();
	UndoSelectedTarget();
	UndoSelectedAttacker();

	Phase = EAttackPhase::Idle;
}

void UStateAttack::UndoSelectedAttacker()
{
	if (GridProxyCurrent) { GridProxyCurrent->SetTargetableEnemies(false); }
	GridProxyCurrent = nullptr;
}

void UStateAttack::UndoSelectedTarget()
{
	if (GridProxyTarget) { GridProxyTarget->SetEnemyTargetTile(false); }
	GridProxyTarget = nullptr;
}

void UStateAttack::UndoSelectedAttackTile()
{
	if (GridProxyMoveTo) { GridProxyMoveTo->SetMoveToTile(false); }
	GridProxyMoveTo == nullptr;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateAttack.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Grid/Manager/GridManager.h"
#include "Grid/GridProxy.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack, "Encounter.Mode.Attack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Attacker, "Grid.State.Attacker");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_CanTargetFrom, "Grid.State.CanTargetFrom");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Target, "Grid.State.Target");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_TargetFrom, "Grid.State.TargetFrom");


UStateAttack::UStateAttack()
{
}

void UStateAttack::Initialize(UGridManager* InGridManager)
{
	GridManager = InGridManager;
	GridManager->OnGridEventStart.AddUObject(this, &ThisClass::Disable);
	GridManager->OnGridEventEnd.AddUObject(this, &ThisClass::Enable);
}

UInputMappingContext* UStateAttack::SetupInputMappingContext(APlayerController* PlayerController)
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);

	IA_Select = NewObject<UInputAction>(this);
	IA_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(IA_Select, EKeys::LeftMouseButton);
	EIC->BindAction(IA_Select, ETriggerEvent::Started, this, &UStateAttack::OnSelect);
	
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
	UE_LOG(LogTemp, Warning, TEXT("OnEnter - ATTACK STATE"));
	
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
		if (GridProxyCurrent->IsAlly() && GridProxyCurrent->HasEnemiesToAttack())
		{
			GridProxyCurrent->SetEnemiesInRange(true);
			Phase = EAttackPhase::SelectedAttacker;
		}
		break;
	case EAttackPhase::SelectedAttacker:
		// check if new player unit selected
		// if (GridProxyCurrent != GridProxy && GridProxy->IsAlly())
		// {
		// 	GridProxyCurrent->UndoAll();
		// 	GridProxyCurrent = GridProxy;
		// 	GridProxyCurrent->SetEnemiesInRange(true);
		// }
		// check if target is enemy that can be attacked, move to next phase
		if (GridProxyCurrent->CanAttack(GridProxy))
		{
			if (GridProxyTarget) { GridProxyTarget->UndoAll(); }
			GridProxyCurrent->SetCanTargetFromTiles(GridProxyTarget, false);
			GridProxyTarget = GridProxy;
			GridProxyCurrent->SetCanTargetFromTiles(GridProxyTarget, true);
			Phase = EAttackPhase::SelectedTarget;
		}
		// check if new enemy target selected
		// if (GridProxyTarget != GridProxy)
		// {
		// 	if (GridProxyTarget) { GridProxyTarget->UndoAll(); }
		// 	GridProxyTarget = GridProxy;
		// }		
		break;
	case EAttackPhase::SelectedTarget:
		// check if new player unit selected, return to previous phase
		// if (GridProxyCurrent != GridProxy && GridProxy->IsAlly())
		// {
		// 	GridProxyCurrent->UndoAll();
		// 	GridProxyCurrent = GridProxy;
		// 	GridProxyCurrent->SetEnemiesInRange(true);
		// 	Phase = EAttackPhase::SelectedAttacker;
		// }
		// check if new enemy target selected, stay on phase
		// else if (GridProxyTarget != GridProxy && GridProxyCurrent->CanAttack(GridProxy) && !GridProxy->IsAlly())
		// {
		// 	GridProxyCurrent->SetCanTargetFromTiles(GridProxyTarget, false);
		// 	GridProxyTarget->UndoAll();
		// 	GridProxyTarget = GridProxy;
		// 	GridProxyCurrent->SetCanTargetFromTiles(GridProxyTarget, true);
		// }
		// check if tile can attack from is selected
		if (GridProxyCurrent->CanAttackFromTile(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			Phase = EAttackPhase::SelectedAttackTile;
		}
		break;
	case EAttackPhase::SelectedAttackTile:
		// check if other tile can attack from is selected, stay on phase
		// if (GridProxyMoveTo != GridProxy && GridProxyTarget->CanAttackFromTile(GridProxy))
		// {
		// 	if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
		// 	GridProxyMoveTo = GridProxy;
		// 	GridProxyMoveTo->SetMoveToTile(true);
		// }
		// TODO: for now hackyyy
		// check if same tile is selected to confirm attack
		if (GridManager->IsMatch(GridProxyMoveTo ,GridProxy))
		{
			GridManager->CreateAttackEvent(GridProxyCurrent, GridProxyMoveTo, GridProxyTarget);
		}
		// THIS IS THE OLD WAY USING CUSTOM MADE EVENT SYSTEM INSTEAD OF Gameplay Ability System
		// {
		// 	TArray<UAbstractEvent*> Events;
		// 	UEventUnitMove* EventMove = NewObject<UEventUnitMove>(this);
		// 	UEventUnitAttack* EventAttack = NewObject<UEventUnitAttack>(this);
		// 	Events.AddUnique(EventMove);
		// 	Events.AddUnique(EventAttack);
		// 	OnEventCreate.Execute(Events);
		// }
		break;
	}

	UE_LOG(LogTemp, Error, TEXT("Mode: Attack - On Select, Phase: %d"), Phase);
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
	GridProxyCurrent->SetEnemiesInRange(true);
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
	if (GridProxyCurrent) { GridProxyCurrent->SetEnemiesInRange(false); }
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
	GridProxyMoveTo = nullptr;
}

void UStateAttack::Enable()
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackState - Enable"));
	if (GridProxyCurrent)
	{
		GridProxyCurrent->SetMoveableTiles(false);
		GridProxyCurrent = nullptr;
	}
	if (GridProxyTarget)
	{
		GridProxyTarget->UndoAll();
		GridProxyTarget = nullptr;
	}
	if (GridProxyMoveTo)
	{
		GridProxyMoveTo->UndoAll();
		GridProxyMoveTo = nullptr;
	}
		
	Phase = EAttackPhase::Idle;
}

void UStateAttack::Disable()
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackState - Disable"));
	Phase = EAttackPhase::None;
}

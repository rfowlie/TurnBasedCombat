// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateMove.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TurnBasedCombat/Public/EventSystem/Events/EventUnitMove.h"
#include "TurnBasedCombat/Public/Grid/GridManager.h"


UStateMove::UStateMove()
{
}

void UStateMove::Initialize(UGridManager* InGridManager)
{
	// only allow setting once?
	if (GridManager == nullptr && InGridManager)
	{
		GridManager = InGridManager;
	}
}

UInputMappingContext* UStateMove::SetupInputMappingContext(APlayerController* PlayerController)
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);
	
	IA_Deselect = NewObject<UInputAction>(this);
	IA_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(IA_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(IA_Deselect, ETriggerEvent::Started, this, &UStateMove::OnDeselect);
	
	IA_Cycle = NewObject<UInputAction>(this);
	IA_Cycle->ValueType = EInputActionValueType::Axis1D;
	FEnhancedActionKeyMapping& Mapping_Cycle_Right = NewInputMappingContext->MapKey(IA_Cycle, EKeys::E);
	FEnhancedActionKeyMapping& Mapping_Cycle_Left = NewInputMappingContext->MapKey(IA_Cycle, EKeys::Q);
	Mapping_Cycle_Left.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	EIC->BindAction(IA_Cycle, ETriggerEvent::Started, this, &UStateMove::OnCycleUnit);
	
	return NewInputMappingContext;
}

void UStateMove::OnEnter()
{
	Super::OnEnter();	
	UE_LOG(LogTemp, Warning, TEXT("EnterInternal - MOVE"));
	Phase = EMovePhase::Idle;
	
}

void UStateMove::OnExit()
{
	Super::OnExit();
	UE_LOG(LogTemp, Warning, TEXT("ExitInternal - MOVE"));
}

void UStateMove::OnSelect()
{
	UGridProxy* GridProxy = GridManager->GetCurrentHoveredGridTile();
	if (!GridProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridProxy is null"));
		return;
	}

	switch(Phase)
	{
	case EMovePhase::None:		
		break;
	case EMovePhase::Idle:
		// check if previous selection, and undo changes
		if (GridProxyCurrent)
		{
			GridProxyCurrent->UndoAll();
		}
		// set new selection
		GridProxyCurrent = GridProxy;
		GridProxyCurrent->SetMoveableTiles(true);
		// if player unit selected move to next phase
		if (GridProxyCurrent->IsPlayer())
		{
			Phase = EMovePhase::SelectedMoveUnit;
		}
		break;
	case EMovePhase::SelectedMoveUnit:
		// check if new player unit selected
		if (GridProxyCurrent != GridProxy && GridProxy->IsPlayer())
		{
			GridProxyCurrent->UndoAll();
			GridProxyCurrent = GridProxy;
			GridProxyCurrent->SetMoveableTiles(true);
		}
		// check if moveable tile selected
		else if (GridProxyCurrent->CanMoveToo(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			Phase = EMovePhase::SelectedMoveTile;
		}
		break;
	case EMovePhase::SelectedMoveUnitNotTeam:
		break;
	case EMovePhase::SelectedMoveTile:
		// check if new player unit selected
		if (GridProxyCurrent != GridProxy && GridProxy->IsPlayer())
		{
			GridProxyCurrent->UndoAll();
			GridProxyCurrent = GridProxy;
			GridProxyCurrent->SetMoveableTiles(true);
			Phase = EMovePhase::SelectedMoveUnit;
		}
		// check if moveable tile selected
		else if (GridProxyCurrent->CanMoveToo(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			Phase = EMovePhase::SelectedMoveTile;
		}
		// check if tile selected again to confirm movement
		else if (GridProxyMoveTo == GridProxy)
		{
			TArray<UAbstractEvent*> Events;
			UEventUnitMove* Event = NewObject<UEventUnitMove>(this);
			Events.AddUnique(Event);
			OnEventCreate.Execute(Events);
		}
		break;
	}
}

void UStateMove::OnDeselect()
{
	switch(Phase)
	{
	case EMovePhase::Idle:
		break;
	case EMovePhase::SelectedMoveUnit:
		if (GridProxyCurrent)
		{
			GridProxyCurrent->SetMoveableTiles(false);
			Phase = EMovePhase::Idle;
		}
		break;
	case EMovePhase::SelectedMoveUnitNotTeam:
		break;
	case EMovePhase::SelectedMoveTile:
		if (GridProxyCurrent)
		{
			GridProxyCurrent->SetMoveToTile(false);
			Phase = EMovePhase::SelectedMoveUnit;
		}
		break;
	default:
		break;
	}
}

void UStateMove::OnCycleUnit()
{
	if (GridProxyCurrent)
	{
		GridProxyCurrent->UndoAll();					
	}
	
	GridProxyCurrent = GridManager->GetNextGridUnit(GridProxyCurrent);
	GridProxyCurrent->SetMoveableTiles(true);
	Phase = EMovePhase::SelectedMoveUnit;
}

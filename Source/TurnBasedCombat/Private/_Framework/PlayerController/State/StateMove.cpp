// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateMove.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TurnBasedCombat/Public/Grid/Manager/GridManager.h"
#include "_Framework/TBC_InfoWorldSubsystem.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Move, "Encounter.Mode.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Move_Idle, "Encounter.Mode.Move.Idle");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Move_SelectedMoveUnit, "Encounter.Mode.Move.SelectedMoveUnit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Move_SelectedMoveTile, "Encounter.Mode.Move.SelectedMoveTile");

UStateMove::UStateMove()
{
}

FGameplayTag UStateMove::GetStateTag() const
{
	return TAG_Encounter_Mode_Move;
}

void UStateMove::Initialize(UGridManager* InGridManager)
{
	GridManager = InGridManager;
	GridManager->OnGridEventStart.AddUObject(this, &ThisClass::Disable);
	GridManager->OnGridEventEnd.AddUObject(this, &ThisClass::Enable);
}

void UStateMove::SetPhase(FGameplayTag InPhaseTag)
{
	PhaseTag = InPhaseTag;
	if (UTBC_InfoWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UTBC_InfoWorldSubsystem>())
	{
		Subsystem->SetPlayerControllerPhase(PhaseTag);
	}
}

UInputMappingContext* UStateMove::SetupInputMappingContext(APlayerController* PlayerController)
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);

	IA_Select = NewObject<UInputAction>(this);
	IA_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(IA_Select, EKeys::LeftMouseButton);
	EIC->BindAction(IA_Select, ETriggerEvent::Started, this, &UStateMove::OnSelect);
	
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
	SetPhase(TAG_Encounter_Mode_Move_Idle);
}

void UStateMove::OnExit()
{
	Super::OnExit();
	UE_LOG(LogTemp, Warning, TEXT("ExitInternal - MOVE"));
}

void UStateMove::OnSelect()
{
	if (Phase == EMovePhase::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move - None"))
		return;
	}
	
	UGridProxy* GridProxy = GridManager->GetCurrentHoveredGridTile();
	if (!GridProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridProxy is null"));
		return;
	}

	switch(Phase)
	{
	case EMovePhase::None:
		UE_LOG(LogTemp, Warning, TEXT("Move - None"))
		break;
	case EMovePhase::Idle:
		UE_LOG(LogTemp, Warning, TEXT("Move - Idle"))
		// check if previous selection, and undo changes
		if (GridProxyCurrent)
		{
			GridProxyCurrent->UndoAll();
		}
		// set new selection
		GridProxyCurrent = GridProxy;

		// check if unit
		if (GridProxyCurrent->HasUnit())
		{
			GridProxyCurrent->SetMoveableTiles(true);
			// GridProxyCurrent->SetState(TAG_Grid_State_Moveable);
		}		
		// if player unit selected move to next phase
		if (GridProxyCurrent->IsAlly())
		{
			Phase = EMovePhase::SelectedMoveUnit;
			SetPhase(TAG_Encounter_Mode_Move_SelectedMoveUnit);
		}
		break;
	case EMovePhase::SelectedMoveUnit:
		UE_LOG(LogTemp, Warning, TEXT("Move - SelectedMoveUnit"))
		// check if new player unit selected
		if (GridProxyCurrent != GridProxy && GridProxy->IsAlly())
		{
			GridProxyCurrent->UndoAll();
			GridProxyCurrent = GridProxy;
			GridProxyCurrent->SetMoveableTiles(true);
			// GridProxyCurrent->SetState(TAG_Grid_State_Moveable);
		}
		// check if moveable tile selected
		else if (GridProxyCurrent != GridProxy && GridProxyCurrent->CanMoveToo(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			// GridProxyCurrent->SetState(TAG_Grid_State_MoveTo);
			Phase = EMovePhase::SelectedMoveTile;
			SetPhase(TAG_Encounter_Mode_Move_SelectedMoveTile);
		}
		break;
	case EMovePhase::SelectedMoveUnitNotTeam:
		break;
	case EMovePhase::SelectedMoveTile:
		// check if tile selected again to confirm movement
		UE_LOG(LogTemp, Warning, TEXT("Move - SelectedMoveTile"))
		// if (GridManager->IsMatch(GridProxyMoveTo ,GridProxy))
		if (GridManager->IsMatch(GridProxyMoveTo, GridProxy))
		{
			GridManager->CreateMoveEvent(GridProxyCurrent, GridProxyMoveTo);
		}
		// check if moveable tile selected
		else if (GridProxyCurrent->CanMoveToo(GridProxy))
		{
			if (GridProxyMoveTo) { GridProxyMoveTo->SetMoveToTile(false); }
			GridProxyMoveTo = GridProxy;
			GridProxyMoveTo->SetMoveToTile(true);
			// GridProxyCurrent->SetState(TAG_Grid_State_MoveTo);
			Phase = EMovePhase::SelectedMoveTile;
			SetPhase(TAG_Encounter_Mode_Move_SelectedMoveTile);
		}
		// check if new player unit selected
		else if (GridProxyCurrent != GridProxy && GridProxy->IsAlly())
		{
			GridProxyCurrent->UndoAll();
			GridProxyCurrent = GridProxy;
			GridProxyCurrent->SetMoveableTiles(true);
			// GridProxyCurrent->SetState(TAG_Grid_State_Moveable);
			Phase = EMovePhase::SelectedMoveUnit;
			SetPhase(TAG_Encounter_Mode_Move_SelectedMoveUnit);
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
		UE_LOG(LogTemp, Warning, TEXT("Move - SelectedMoveUnit - Deselect"))
		if (GridProxyCurrent)
		{
			GridProxyCurrent->SetMoveableTiles(false);
			Phase = EMovePhase::Idle;
			SetPhase(TAG_Encounter_Mode_Move_Idle);
		}
		break;
	case EMovePhase::SelectedMoveUnitNotTeam:
		break;
	case EMovePhase::SelectedMoveTile:
		UE_LOG(LogTemp, Warning, TEXT("Move - SelectedMoveTile - Deselect"))
		if (GridProxyCurrent)
		{
			GridProxyCurrent->SetMoveToTile(false);
			Phase = EMovePhase::SelectedMoveUnit;
			SetPhase(TAG_Encounter_Mode_Move_SelectedMoveUnit);
		}
		break;
	default:
		break;
	}
}

void UStateMove::OnCycleUnit()
{
	// TODO: does not return anything at the moment
	// if (GridProxyCurrent)
	// {
	// 	GridProxyCurrent->UndoAll();					
	// }
	//
	// GridProxyCurrent = GridManager->GetNextGridUnit(GridProxyCurrent);
	// GridProxyCurrent->SetMoveableTiles(true);
	// Phase = EMovePhase::SelectedMoveUnit;
}

void UStateMove::Enable()
{
	UE_LOG(LogTemp, Warning, TEXT("UStateMove - Enable"));
	if (GridProxyCurrent)
	{
		GridProxyCurrent->SetMoveableTiles(false);
		GridProxyCurrent = nullptr;
	}
	// if (GridProxyMoveTo)
	// {
	// 	GridProxyMoveTo->SetMoveToTile(false);
	// }
		
	Phase = EMovePhase::Idle;
	SetPhase(TAG_Encounter_Mode_Move_Idle);
}

void UStateMove::Disable()
{
	UE_LOG(LogTemp, Warning, TEXT("UStateMove - Disable"));
	Phase = EMovePhase::None;
}

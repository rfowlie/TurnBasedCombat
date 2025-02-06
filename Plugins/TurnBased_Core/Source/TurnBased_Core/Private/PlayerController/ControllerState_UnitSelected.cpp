// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitSelected.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "TurnBased_Core_Tags.h"
#include "Grid/GridWorldSubsystem.h"
#include "PlayerController/ControllerState_Idle.h"
#include "PlayerController/ControllerState_UnitChooseAction.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"



UControllerState_UnitSelected::UControllerState_UnitSelected()
{
}

UControllerState_UnitSelected* UControllerState_UnitSelected::Create(AGridUnit* InSelectedUnit, bool IsPlayerUnit)
{
	if (!IsValid(InSelectedUnit)) { return nullptr; }
	UControllerState_UnitSelected* Object = NewObject<UControllerState_UnitSelected>();
	Object->SelectedUnit = InSelectedUnit;
	Object->IsPlayerUnit = IsPlayerUnit;

	return Object;
}

void UControllerState_UnitSelected::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// if (!bSuccess) { OnChangedDelegate.Execute(UControllerState_Idle::Create()); }
	if (!SetMovementTiles()) { PlayerController->PopState(); }
}

void UControllerState_UnitSelected::OnExit()
{
	Super::OnExit();

	// revert tiles back to
	if (UGridWorldSubsystem* Subsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		for (auto GridMovement : GridMovements)
		{
			if (AGridUnit* GridUnit = Subsystem->GetGridUnitOnTile(GridMovement.GridTile))
			{
				GridUnit->SetState(TAG_TBCore_Grid_State_Idle);
			}
			
			GridMovement.GridTile.Get()->SetState(TAG_TBCore_Grid_State_Idle);
		}
		
		if (IsPlayerUnit)
		{
			// stop cursor follow
			Subsystem->OnGridTileHoveredStart.RemoveDynamic(this, &ThisClass::MoveSelectedTarget);
			
			// move selected target back to original location
			SelectedUnit->SetActorLocation(SelectedUnitTile->GetPlacementLocation());
		}
	}		
}

UInputMappingContext* UControllerState_UnitSelected::CreateInputMappingContext()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);

	InputAction_Select = NewObject<UInputAction>(this);
	InputAction_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(InputAction_Select, EKeys::LeftMouseButton);
	EIC->BindAction(InputAction_Select, ETriggerEvent::Started, this, &ThisClass::OnSelect);
	
	InputAction_Deselect = NewObject<UInputAction>(this);
	InputAction_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(InputAction_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(InputAction_Deselect, ETriggerEvent::Started, this, &ThisClass::OnDeselect);
	
	InputAction_CycleUnit = NewObject<UInputAction>(this);
	InputAction_CycleUnit->ValueType = EInputActionValueType::Axis1D;
	FEnhancedActionKeyMapping& Mapping_Cycle_Right = NewInputMappingContext->MapKey(InputAction_CycleUnit, EKeys::E);
	FEnhancedActionKeyMapping& Mapping_Cycle_Left = NewInputMappingContext->MapKey(InputAction_CycleUnit, EKeys::Q);
	Mapping_Cycle_Left.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	EIC->BindAction(InputAction_CycleUnit, ETriggerEvent::Started, this, &ThisClass::OnCycleUnit);
	
	return NewInputMappingContext;
}

// TODO: this is a mess right now
void UControllerState_UnitSelected::OnSelect()
{
	if (UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		AGridTile* SelectedGridTile = GridSubsystem->GetGridTileHovered();

		if (IsPlayerUnit)
		{
			for (FGridMovement GridMovement : GridMovements)
			{
				if (GridMovement.GridTile.Get() == SelectedGridTile)
				{
					// stop cursor follow
					GridSubsystem->OnGridTileHoveredStart.RemoveDynamic(this, &ThisClass::MoveSelectedTarget);
					PlayerController->PushState(UControllerState_UnitChooseAction::Create(SelectedUnit), false);
				}
			}
		}
		else
		{
			// check if a different unit is selected
			if (AGridUnit* UnitOnTile = GridSubsystem->GetGridUnitOnTile(SelectedGridTile))
			{
				if (UTurnWorldSubsystem* TurnSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
				{
					// TODO: need to decide how we want to handle selecting player/enemy unit
					// OnChangedDelegate.Execute(UControllerState_UnitSelected::Create(
					// 	UnitOnTile, TurnSubsystem->CanUnitTakeAction(UnitOnTile)));

					// this will ensure that the movement tiles are not removed when moving to next state
					PlayerController->PushState(UControllerState_UnitSelected::Create(
						UnitOnTile, TurnSubsystem->CanUnitTakeAction(UnitOnTile)), true);
				}
			}
		}		
	}
}

void UControllerState_UnitSelected::OnDeselect()
{
	PlayerController->PopState();
}

void UControllerState_UnitSelected::OnCycleUnit()
{
	// get next unit from turn manager
	if (UTurnWorldSubsystem* Subsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
	{
		// if unit not the same then shift
		AGridUnit* CycleUnit = Subsystem->GetNextUnit(SelectedUnit);
		if (CycleUnit != SelectedUnit)
		{			
			// OnChangedDelegate.Execute(UControllerState_UnitSelected::Create(CycleUnit, IsPlayerUnit));

			OnExit();
			SelectedUnit = CycleUnit;
			SetMovementTiles();
		}
		
		// TODO: have camera focus on selected unit
	}
}

bool UControllerState_UnitSelected::SetMovementTiles()
{
	bool bSuccess = false;
	if (IsValid(SelectedUnit))
	{
		if (UGridWorldSubsystem* Subsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
		{
			Subsystem->CalculateGridMovement(GridMovements, SelectedUnit);
			for (auto GridMovement : GridMovements)
			{
				if (AGridUnit* GridUnit = Subsystem->GetGridUnitOnTile(GridMovement.GridTile))
				{
					GridUnit->SetState(TAG_TBCore_Grid_Tile_CanAttack);
					GridMovement.GridTile.Get()->SetState(TAG_TBCore_Grid_Tile_CanAttack);
				}
				else
				{
					GridMovement.GridTile.Get()->SetState(TAG_TBCore_Grid_Tile_CanMove);
				}
			}

			if (IsPlayerUnit)
			{
				// cache starting tile
				SelectedUnitTile = Subsystem->GetGridTileOfUnit(SelectedUnit);

				// bind to on tile hovered to enable moving the character around
				Subsystem->OnGridTileHoveredStart.AddUniqueDynamic(this, &ThisClass::MoveSelectedTarget);				
			}

			bSuccess = true;
		}			
	}

	return bSuccess;
}

void UControllerState_UnitSelected::MoveSelectedTarget(AGridTile* InGridTile)
{
	if (IsValid(InGridTile))
	{
		for (FGridMovement GridMovement : GridMovements)
		{
			if (GridMovement.GridTile == InGridTile)
			{
				SelectedUnit->SetActorLocation(InGridTile->GetPlacementLocation());
				break;
			}
		}		
	}
}

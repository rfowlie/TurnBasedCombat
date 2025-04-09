// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitSelected.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "TurnBased_Core_Tags.h"
#include "Grid/GridWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Attack_TargetSelected.h"
#include "PlayerController/ControllerState_Idle.h"
#include "PlayerController/ControllerState_OnUnitMove.h"
#include "PlayerController/ControllerState_UnitChooseAction.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"



UControllerState_UnitSelected::UControllerState_UnitSelected()
{
}

UControllerState_UnitSelected* UControllerState_UnitSelected::Create(AGridUnit* InActiveUnit, const int32 InAvailableMovement)
{
	if (!IsValid(InActiveUnit)) { return nullptr; }
	UControllerState_UnitSelected* Object = NewObject<UControllerState_UnitSelected>();
	Object->ActiveUnit = InActiveUnit;
	Object->AvailableMovement = InAvailableMovement;
	
	return Object;
}

void UControllerState_UnitSelected::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	UTurnWorldSubsystem* TurnSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (!TurnSubsystem) { PlayerController->PopState(); }
	IsPlayerUnit = TurnSubsystem->CanUnitTakeAction(ActiveUnit);
	if (!SetMovementTiles()) { PlayerController->PopState(); }

	UGridWorldSubsystem* GridWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	if (!GridWorldSubsystem) { PlayerController->PopState(); }
	SelectedUnitTile = GridWorldSubsystem->GetGridTileOfUnit(ActiveUnit);

	// update cursor
	if (APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
	{
		Pawn->SetFollowCursor();
	}
}

void UControllerState_UnitSelected::OnExit()
{
	Super::OnExit();

	UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	if (!GridSubsystem) { return; }
	
	// revert movement tiles
	for (auto GridMovement : GridMovements)
	{
		if (AGridUnit* GridUnit = GridSubsystem->GetGridUnitOnTile(GridMovement.GridTile))
		{
			GridUnit->SetState(TAG_TBCore_Grid_State_Idle);
		}
			
		GridMovement.GridTile.Get()->SetState(TAG_TBCore_Grid_State_Idle);
	}

	// revert attack tiles
	for (auto EnemyUnit : EnemyUnitsInRange)
	{
		// should alwasy return something...
		GridSubsystem->GetGridTileOfUnit(EnemyUnit)->SetState(TAG_TBCore_Grid_State_Idle);
		EnemyUnit->SetState(TAG_TBCore_Grid_State_Idle);
	}
		
	if (IsPlayerUnit)
	{
		// // stop cursor follow
		// GridSubsystem->OnGridTileHoveredStart.RemoveDynamic(this, &ThisClass::MoveSelectedTarget);
		// 	
		// // move selected target back to original location
		// ActiveUnit->SetActorLocation(SelectedUnitTile->GetPlacementLocation());
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
	UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	if (!GridSubsystem) { return; }

	AGridTileBase* SelectedTile = GridSubsystem->GetGridTileHovered();

	// check attack tile selected, ALLOW THIS EVEN FOR NON PLAYER UNITS, NEXT STATE WILL BLOCK
	AGridUnit* TargetUnit = GridSubsystem->GetGridUnitOnTile(SelectedTile);
	if (IsValid(TargetUnit) && EnemyUnitsInRange.Contains(TargetUnit))
	{
		PlayerController->PushState(UControllerState_Attack_TargetSelected::Create(ActiveUnit, TargetUnit, GridMovements), true);
	}
	// check movement tile selected
	else if (IsPlayerUnit)
	{
		// check if self selected, show actions menu
		if (TargetUnit == ActiveUnit)
		{
			// move to confirm movement state, should this just be here for now?
			PlayerController->PushState(UControllerState_UnitChooseAction::Create(ActiveUnit, SelectedTile), true);
		}
		else
		{
			for (FGridMovement GridMovement : GridMovements)
			{
				if (GridMovement.GridTile.Get() == SelectedTile)
				{
					// stop cursor follow
					// GridSubsystem->OnGridTileHoveredStart.RemoveDynamic(this, &ThisClass::MoveSelectedTarget);
					// TODO: in the future, we can think about chaining movement together to follow a set path, etc.
					// PlayerController->PushState(UControllerState_UnitSelected::Create(
					// 	ActiveUnit, AvailableMovement - GridMovement.Cost), true);
					PlayerController->PushState(UControllerState_UnitChooseAction::Create(ActiveUnit, SelectedTile), true);
				}
			}
		}		
	}
	else
	{
		// check if a different unit is selected
		if (AGridUnit* SelectedUnit = GridSubsystem->GetGridUnitOnTile(SelectedTile))
		{
			if (UTurnWorldSubsystem* TurnSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
			{
				// TODO: need to decide how we want to handle selecting player/enemy unit
				// OnChangedDelegate.Execute(UControllerState_UnitSelected::Create(
				// 	UnitOnTile, TurnSubsystem->CanUnitTakeAction(UnitOnTile)));

				// this will ensure that the movement tiles are not removed when moving to next state
				PlayerController->PushState(UControllerState_UnitSelected::Create(SelectedUnit, SelectedUnit->GetAvailableMovement()), true);
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
		AGridUnit* CycleUnit = Subsystem->GetNextUnit(ActiveUnit);
		if (CycleUnit != ActiveUnit)
		{
			OnExit();
			ActiveUnit = CycleUnit;
			SetMovementTiles();
		}
		
		// TODO: have camera focus on selected unit
	}
}

bool UControllerState_UnitSelected::SetMovementTiles()
{
	if (!IsValid(ActiveUnit)) { return false; }
	UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	UTurnWorldSubsystem* TurnSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (!GridSubsystem || !TurnSubsystem) { return false; }
	
	// calculate movement
	GridSubsystem->CalculateGridMovement(GridMovements, ActiveUnit, AvailableMovement);
	for (auto GridMovement : GridMovements)
	{
		if (AGridUnit* GridUnit = GridSubsystem->GetGridUnitOnTile(GridMovement.GridTile))
		{
			GridUnit->SetState(TAG_TBCore_Grid_Tile_CanAttack);
			GridMovement.GridTile.Get()->SetState(TAG_TBCore_Grid_Tile_CanAttack);
		}
		else
		{
			GridMovement.GridTile.Get()->SetState(TAG_TBCore_Grid_Tile_CanMove);
		}
	}

	// calculate targetable enemies
	TArray<AGridUnit*> EnemyGridUnits;
	TurnSubsystem->GetFactionEnemies(ActiveUnit, EnemyGridUnits);
	for (const auto EnemyUnit : EnemyGridUnits)
	{
		TMap<AGridTileBase*, int32> AttackTiles;
		GridSubsystem->CalculateGridAttackTiles(AttackTiles, GridMovements, ActiveUnit, EnemyUnit);

		// if there are tiles in movement that can be used to attack this unit, set state
		if (!AttackTiles.IsEmpty())
		{
			EnemyUnitsInRange.Add(EnemyUnit);
			GridSubsystem->GetGridTileOfUnit(EnemyUnit)->SetState(TAG_TBCore_Grid_Tile_CanAttack);
			EnemyUnit->SetState(TAG_TBCore_Grid_Tile_CanAttack);
		}
	}

	// 
	if (IsPlayerUnit)
	{
		// TODO: decide if we want the player to move around with the cursor
		// // cache starting tile
		// SelectedUnitTile = Subsystem->GetGridTileOfUnit(SelectedUnit);
		//
		// // bind to on tile hovered to enable moving the character around
		// Subsystem->OnGridTileHoveredStart.AddUniqueDynamic(this, &ThisClass::MoveSelectedTarget);				
	}

	return true;
}

void UControllerState_UnitSelected::MoveSelectedTarget(AGridTile* InGridTile)
{
	if (IsValid(InGridTile))
	{
		for (FGridMovement GridMovement : GridMovements)
		{
			if (GridMovement.GridTile == InGridTile)
			{
				ActiveUnit->SetActorLocation(InGridTile->GetPlacementLocation());
				break;
			}
		}		
	}
}

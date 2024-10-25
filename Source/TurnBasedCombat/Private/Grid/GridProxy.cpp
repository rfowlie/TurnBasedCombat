// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/GridProxy.h"

#include "Grid/GridStructs.h"
#include "Grid/GridUtility.h"
#include "Grid/Tile/GridTile.h"
#include "Grid/Unit/GridUnit.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Move, "Encounter.Mode.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Idle, "Grid.State.Idle");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Mover, "Grid.State.Mover");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Moveable, "Grid.State.Moveable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_MoveTo, "Grid.State.MoveTo");


UGridProxy* UGridProxy::CreateGridProxy(UObject* Outer,
		UTurnManager* InTurnManager,
		AGridTile* InGridTile,
		AGridUnit* InGridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements)
{
	UGridProxy* GridProxy = NewObject<UGridProxy>(Outer);
	if (GridProxy)
	{
		GridProxy->Init(
			InTurnManager,
			InGridTile,
			InGridUnit,
			InMovementDelegate,
			InGridMovements);
	}

	return GridProxy;
}

// void UGridProxy::SetState(FGameplayTag State)
// {
// 	if (GridTile) { GridTile->SetState(State); }
// 	if (GridUnit) { GridUnit->SetState(State); }
// }

void UGridProxy::UndoAll()
{
	// TODO: how this work...
	GridTile->SetState(TAG_Grid_State_Idle);
	for (auto GridMovement : GridMovements)
	{
		GridMovement.GridTile.Get()->SetState(TAG_Grid_State_Idle);
	}
}

void UGridProxy::SetMoveableTiles(bool Activate)
{
	// if (CalculateGridMovementDelegate.IsBound())
	// {
	// 	CalculateGridMovementDelegate.Execute(GridMovements, GridUnit);
	// }

	for (auto GridMovement : GridMovements)
	{
		if (Activate)
		{
			GridMovement.GridTile.Get()->SetState(TAG_Grid_State_Moveable);
		}
		else
		{
			GridMovement.GridTile.Get()->SetState(TAG_Grid_State_Idle);
		}		
	}

	if (Activate)
	{
		GridTile->SetState(TAG_Grid_State_Mover);
	}
	else
	{
		GridTile->SetState(TAG_Grid_State_Idle);
	}	
}

void UGridProxy::SetTargetableEnemies(bool Activate)
{
}

void UGridProxy::SetMoveToTile(bool Activate)
{
	Activate ? GridTile->SetState(TAG_Grid_State_MoveTo) : GridTile->SetState(TAG_Grid_State_Moveable);
}

void UGridProxy::SetEnemyTargetTile(bool Activate)
{
}

void UGridProxy::SetAttackTiles(UGridProxy* GridProxy, bool Activate)
{
}

bool UGridProxy::CanMoveToo(UGridProxy* GridProxy)
{
	// TODO: seems hacky...
	FGridPosition ProxyGridPosition = GridProxy->GetGridPosition();
	for (auto GridMovement : GridMovements)
	{
		if (GridMovement.GridPosition == ProxyGridPosition)
		{
			return true;
		}
	}
	
	return false;
}

bool UGridProxy::CanAttack(UGridProxy* GridProxy)
{
	return false;
}

bool UGridProxy::HasUnit() const
{
	return IsValid(GridUnit);
}

bool UGridProxy::IsAlly(UGridProxy* GridProxy)
{
	return false;
}

bool UGridProxy::IsPlayer()
{
	// TODO: for now
	return IsValid(GridUnit);
}

bool UGridProxy::IsEnemy()
{
	return false;
}

bool UGridProxy::IsMoveTile(UGridProxy* GridProxy)
{
	return false;
}

bool UGridProxy::IsAttackTile(UGridProxy* GridProxy)
{
	return false;
}

FGridPosition UGridProxy::GetGridPosition() const
{
	return UGridUtility::CalculateGridPosition(GridTile);
}

UGridProxy::UGridProxy()
{
}

void UGridProxy::Init(
	UTurnManager* InTurnManager,
	AGridTile* InGridTile,
	AGridUnit* InGridUnit,
	const FCalculateGridMovement& InMovementDelegate,
	TArray<FGridMovement> InGridMovements)
{
	TurnManager = InTurnManager;
	GridTile = InGridTile;
	GridUnit = InGridUnit;
	CalculateGridMovementDelegate = InMovementDelegate;
	GridMovements = InGridMovements;
}

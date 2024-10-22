// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/GridProxy.h"

#include "Grid/GridStructs.h"
#include "Grid/Tile/GridTile.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Move, "Encounter.Mode.Move");
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

void UGridProxy::UndoAll()
{
}

void UGridProxy::SetMoveableTiles(bool Activate)
{
	TArray<FGridMovement> GridMovements;
	CalculateGridMovementDelegate.Execute(GridMovements, GridUnit);
	for (auto GridMovement : GridMovements)
	{
		GridMovement.GridTile.Get()->SetState(TAG_Grid_State_Moveable);
	}

	GridTile->SetState(TAG_Grid_State_Mover);
}

void UGridProxy::SetTargetableEnemies(bool Activate)
{
}

void UGridProxy::SetMoveToTile(bool Activate)
{
}

void UGridProxy::SetEnemyTargetTile(bool Activate)
{
}

void UGridProxy::SetAttackTiles(UGridProxy* GridProxy, bool Activate)
{
}

bool UGridProxy::CanMoveToo(UGridProxy* GridProxy)
{
	return false;
}

bool UGridProxy::CanAttack(UGridProxy* GridProxy)
{
	return false;
}

bool UGridProxy::IsAlly(UGridProxy* GridProxy)
{
	return false;
}

bool UGridProxy::IsPlayer()
{
	// TODO: check some static registry???
	return false;
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

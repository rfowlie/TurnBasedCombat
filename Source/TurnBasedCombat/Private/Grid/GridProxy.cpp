// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/GridProxy.h"

#include "Grid/GridStructs.h"
#include "Grid/GridHelper.h"
#include "Grid/Manager/TurnManager.h"
#include "Tile/GridTile.h"
#include "Unit/GridUnit.h"
#include "Item/WeaponDataAsset_OLD.h"
#include "_Framework/PlayerController/State/StateAttack.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Idle, "Grid.State.Idle");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Mover, "Grid.State.Mover");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Moveable, "Grid.State.Moveable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_MoveTo, "Grid.State.MoveTo");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_CanAttack, "Grid.State.CanAttack");


UGridProxy* UGridProxy::CreateGridProxy(UObject* Outer,
		UTurnManager* InTurnManager,
		AGridTile* InGridTile,
		AGridUnit* InGridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements,
		TArray<FGridPair> InEnemyUnitsInRange)
{
	UGridProxy* GridProxy = NewObject<UGridProxy>(Outer);
	if (GridProxy)
	{
		// GridProxy->Init(
		// 	InTurnManager,
		// 	InGridTile,
		// 	InGridUnit,
		// 	InMovementDelegate,
		// 	InGridMovements,
		// 	InEnemyUnitsInRange);

		GridProxy->TurnManager = InTurnManager;
		GridProxy->GridTile = InGridTile;
		GridProxy->GridUnit = InGridUnit;
		GridProxy->CalculateGridMovementDelegate = InMovementDelegate;
		GridProxy->GridMovements = InGridMovements;
		GridProxy->EnemyGridUnitsInRange = InEnemyUnitsInRange;
	}

	return GridProxy;
}

UAbilitySystemComponent* UGridProxy::GetAbilitySystemComponent() const
{
	return GridUnit ? UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GridUnit) : nullptr;
}

void UGridProxy::UndoAll()
{
	// TODO: how this work...
	GridTile->SetState(TAG_Grid_State_Idle);
	for (auto GridMovement : GridMovements)
	{
		GridMovement.GridTile.Get()->SetState(TAG_Grid_State_Idle);
	}
	for (auto GridPair : EnemyGridUnitsInRange)
	{
		GridPair.GridTile->SetState(TAG_Grid_State_Idle);
		GridPair.GridUnit->SetState(TAG_Grid_State_Idle);
	}
}

void UGridProxy::SetMoveableTiles(bool Activate)
{
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

void UGridProxy::SetEnemiesInRange(bool Activate)
{
	if (EnemyGridUnitsInRange.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Attack State: current unit has no enemies in range"))
		return; 
	}
	
	for (auto GridPair : EnemyGridUnitsInRange)
	{
		Activate ? GridPair.GridTile->SetState(TAG_Grid_State_CanAttack) : GridPair.GridTile->SetState(TAG_Grid_State_Idle);
		Activate ? GridPair.GridUnit->SetState(TAG_Grid_State_CanAttack) : GridPair.GridUnit->SetState(TAG_Grid_State_Idle);
	}
}

void UGridProxy::SetMoveToTile(bool Activate)
{
	Activate ? GridTile->SetState(TAG_Grid_State_MoveTo) : GridTile->SetState(TAG_Grid_State_Moveable);
}

void UGridProxy::SetEnemyTargetTile(bool Activate)
{
}

bool UGridProxy::SetCanTargetFromTiles(UGridProxy* Other, bool Activate)
{
	// TODO: set the tiles which this unit can attack the given proxy from
	// should also determine which weapons or skills from which tile so we can show UI information
	if (Other == nullptr) { return false; }

	// TODO: might cause bad behaviour
	// reset state for potential previous tiles
	for (FGridMovement AttackTile : CurrentCanAttackFromTiles)
	{
		AttackTile.GridTile->SetState(TAG_Grid_State_Idle);
	}
	
	CurrentCanAttackFromTiles.Empty();
	for (auto GridMovement: GridMovements)
	{
		// for (const int32 WeaponRange : GridUnit->GetWeaponRanges())
		// {
		// 	if (UGridHelper::GetDistanceBetweenGridPositions(
		// 	UGridHelper::CalculateGridPosition(GridMovement.GridTile),
		// 	Other->GetGridPosition()) == WeaponRange)
		// 	{
		// 		CurrentCanAttackFromTiles.AddUnique(GridMovement);
		// 		break;
		// 	}
		// }
	}

	// set state for tiles
	for (FGridMovement AttackTile : CurrentCanAttackFromTiles)
	{
		if (Activate)
		{
			AttackTile.GridTile->SetState(TAG_Tile_State_CanTargetFrom);
		}
		else
		{
			AttackTile.GridTile->SetState(TAG_Grid_State_Idle);
		}
		// Activate ? AttackTile.GridTile->SetState(TAG_Tile_State_CanTargetFrom) : AttackTile.GridTile->SetState(TAG_Grid_State_Idle);
	}
	
	// remove state for other enemies??? PROBABLY won't matter as there won't be tile overlap

	return true;
}

bool UGridProxy::CanMoveToo(UGridProxy* GridProxy)
{
	// TODO: seems hacky...
	FGridPosition ProxyGridPosition = GridProxy->GetGridPosition();
	for (auto GridMovement : GridMovements)
	{
		if (UGridHelper::CalculateGridPosition(GridMovement.GridTile) == ProxyGridPosition)
		{
			return true;
		}
	}
	
	return false;
}

bool UGridProxy::CanAttack(UGridProxy* Other)
{
	FGridPosition OtherGridPosition = Other->GetGridPosition();
	for (auto EnemyUnitInRange : EnemyGridUnitsInRange)
	{
		if (OtherGridPosition == UGridHelper::CalculateGridPosition(EnemyUnitInRange.GridUnit))
		{
			return true;
		}
	}
	
	return false;
}

bool UGridProxy::HasUnit() const
{
	return IsValid(GridUnit);
}

bool UGridProxy::IsAlly() const
{
	if (GridUnit == nullptr) { return false; }
	return GridUnit->Execute_GetFaction(GridUnit) == TurnManager->GetCurrentFaction();
}

bool UGridProxy::IsMoveTile(UGridProxy* Other)
{
	// TODO...
	return false;
}

bool UGridProxy::CanAttackFromTile(UGridProxy* Other) const
{
	for (auto GridMovement : CurrentCanAttackFromTiles)
	{
		if (UGridHelper::CalculateGridPosition(GridMovement.GridTile) == Other->GetGridPosition())
		{
			return true;
		}
	}

	return false;
}

bool UGridProxy::GetValidWeaponsToAttackWith(UGridProxy* Other, TArray<UWeaponDataAsset_OLD*> ValidWeapons) const
{
	if (!CanAttackFromTile(Other)) { return false; }

	// find out what weapons can be used from other proxy positions
	int32 Distance = UGridHelper::GetDistanceBetweenGridPositions(
		GetGridPosition(), Other->GetGridPosition());
	// for (auto Weapon : GridUnit->WeaponDataAssets)
	// {
	// 	if (Weapon->WeaponStats.Range == Distance)
	// 	{
	// 		ValidWeapons.Add(Weapon);
	// 	}
	// }

	return true;
}

bool UGridProxy::HasEnemiesToAttack() const
{
	// TODO: might be janky, we have to ensure that there are no empty grid pairs...
	return !EnemyGridUnitsInRange.IsEmpty();
}

FVector UGridProxy::GetWorldLocation() const
{
	return GridTile->GetActorLocation();
}

FGridPosition UGridProxy::GetGridPosition() const
{
	return UGridHelper::CalculateGridPosition(GridTile);
}

FGameplayTag UGridProxy::GetFaction() const
{
	// this seems sus...
	// grid unit can be null
	return GridUnit == nullptr ? FGameplayTag() : GridUnit->Execute_GetFaction(GridUnit);
}

UGridProxy::UGridProxy()
{
}

// void UGridProxy::Init(
// 	UTurnManager* InTurnManager,
// 	AGridTile* InGridTile,
// 	AGridUnit* InGridUnit,
// 	const FCalculateGridMovement& InMovementDelegate,
// 	TArray<FGridMovement> InGridMovements,
// 	TArray<FGridPair> InEnemyUnitsInRange)
// {
// 	TurnManager = InTurnManager;
// 	GridTile = InGridTile;
// 	GridUnit = InGridUnit;
// 	CalculateGridMovementDelegate = InMovementDelegate;
// 	GridMovements = InGridMovements;
// 	EnemyGridUnitsInRange = InEnemyUnitsInRange;
// }

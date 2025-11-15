// Fill out your copyright notice in the Description page of Project Settings.


#include "Interpreter/GridPositionBasedInterpreterComponent.h"
#include "Library/GridMechanics_GridPositionLibrary.h"
#include "Subsystem/GridTrackerSubsystem.h"
#include "Tile/GridTileBase.h"
#include "Unit/GridUnitBase.h"


void UGridPositionBasedInterpreterComponent::UpdateTileMapping(AGridTileBase* GridTile)
{
	if (!IsValid(GridTile)) { return; }
	
	// remove mapping for grid unit
	LocationGridTileMap.Remove(GridTileLocationMap[GridTile]);
	GridTileLocationMap.Remove(GridTile);

	// update the unit that has moved	
	const FGridPosition GridPosition = UGridMechanics_GridPositionLibrary::CalculateGridPositionFromSize(GridTile, GridSize);
	LocationGridTileMap.Add(GridPosition, GridTile);
	GridTileLocationMap.Add(GridTile, GridPosition);
}

void UGridPositionBasedInterpreterComponent::UpdateTileMappingsAll()
{
	if (UGridTrackerSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridTrackerSubsystem>())
	{
		for (AGridTileBase* GridTile : Subsystem->GetAllGridTiles())
		{
			UpdateTileMapping(GridTile);
		}
	}	
}

void UGridPositionBasedInterpreterComponent::UpdateUnitMapping(AGridUnitBase* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }
	
	// remove mapping for grid unit
	LocationGridUnitMap.Remove(GridUnitLocationMap[GridUnit]);
	GridUnitLocationMap.Remove(GridUnit);

	// update the unit that has moved	
	const FGridPosition GridPosition = UGridMechanics_GridPositionLibrary::CalculateGridPositionFromSize(GridUnit, GridSize);
	LocationGridUnitMap.Add(GridPosition, GridUnit);
	GridUnitLocationMap.Add(GridUnit, GridPosition);
}

void UGridPositionBasedInterpreterComponent::UpdateUnitMappingsAll()
{
	if (UGridTrackerSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridTrackerSubsystem>())
	{
		for (AGridUnitBase* GridUnit : Subsystem->GetAllGridUnits())
		{
			UpdateUnitMapping(GridUnit);
		}
	}	
}

AGridUnitBase* UGridPositionBasedInterpreterComponent::GetGridUnitOnTile(const AGridTileBase* GridTile) const
{
	if (GridTileLocationMap.Contains(GridTile))
	{
		if (LocationGridUnitMap.Contains(GridTileLocationMap[GridTile]))
		{
			return LocationGridUnitMap[GridTileLocationMap[GridTile]];
		}
	}

	return nullptr;
}

AGridTileBase* UGridPositionBasedInterpreterComponent::GetGridTileOfUnit(const AGridUnitBase* GridUnit) const
{
	if (GridUnitLocationMap.Contains(GridUnit))
	{
		if (LocationGridTileMap.Contains(GridUnitLocationMap[GridUnit]))
		{
			return LocationGridTileMap[GridUnitLocationMap[GridUnit]];
		}
	}
		
	return nullptr;
}

bool UGridPositionBasedInterpreterComponent::CanUnitBeOnTile(AGridUnitBase* GridUnit, AGridTileBase* GridTile)
{
	if (!IsValid(GridUnit) || !IsValid(GridTile)) { return false; }

	return true;
}

int32 UGridPositionBasedInterpreterComponent::InterpretMovementCost(AGridUnitBase* GridUnit, AGridTileBase* GridTile)
{
	if (!IsValid(GridUnit) || !IsValid(GridTile)) { return 1; }
	return 1;
}

TArray<AGridTileBase*> UGridPositionBasedInterpreterComponent::GetGridTilesAtRange(
	FGridPosition StartGridPosition, const int32 Range)
{
	TArray<AGridTileBase*> Output;
	TArray<FGridPosition> PositionsAtRange;
	UGridMechanics_GridPositionLibrary::GetGridPositionsAtRange(StartGridPosition, Range, PositionsAtRange);
	for (FGridPosition GridLocation : PositionsAtRange)
	{
		if (LocationGridTileMap.Contains(GridLocation))
		{
			Output.Add(LocationGridTileMap[GridLocation]);
		}
	}

	return Output;
}

TArray<AGridTileBase*> UGridPositionBasedInterpreterComponent::GetGridTilesAtRanges(
	const FGridPosition StartGridPosition, TArray<int32> Ranges)
{
	TSet UniqueRanges(Ranges);
	for (const int32 Range : Ranges)
	{
		UniqueRanges.Add(FMath::Abs(Range));
	}

	TSet<AGridTileBase*> Output;
	for (const int32 Range : UniqueRanges)
	{
		Output.Append(GetGridTilesAtRange(StartGridPosition, Range));
	}

	return Output.Array();
}

void UGridPositionBasedInterpreterComponent::CalculateGridMovement(TArray<FGridMovement>& OutMovement,
	AGridUnitBase* GridUnit, const int32 AvailableMovement)
{
	if (!GridUnitLocationMap.Contains(GridUnit))
	{
		// GridUnit does not have a location...
		UE_LOG(LogTemp, Error, TEXT("Grid unit does not have location!!"));
		return;
	}

	if (!LocationGridTileMap.Contains(GridUnitLocationMap[GridUnit]))
	{
		// GridTile of GridUnit does not have a location...
		UE_LOG(LogTemp, Error, TEXT("Grid Tile Map does not contain Units location!!"));
		return;
	}
	
	AGridTileBase* StartTile = LocationGridTileMap[GridUnitLocationMap[GridUnit]];    
	if (!IsValid(StartTile))
	{
		// UE_LOG(LogTemp, Error, TEXT("Tile Actor At Location (X:%d, Y:%d) is null!!"), StartGridPosition.X, StartGridPosition.Y);
		UE_LOG(LogTemp, Error, TEXT("No grid tile at unit location!!"));
		return;
	}

	// for now just make sure it is clear, might want to add boolean to check to do this...
	OutMovement.Empty();
 
	// ALGORITHM
	TSet<FGridMovement> Processed;
	TQueue<FGridMovement> ProcessingQueue;
	
	FGridMovement StartNode;
	StartNode.Cost = 0;
	StartNode.GridTile = StartTile;    	
	ProcessingQueue.Enqueue(StartNode);
 
	FGridMovement Current;
	while (!ProcessingQueue.IsEmpty())
	{		
		ProcessingQueue.Dequeue(Current);
		Processed.Add(Current);
    		
		for (AGridTileBase* TargetTile : GetGridTilesAtRange(GridTileLocationMap[Current.GridTile], 1))
		{
			if (GetGridUnitOnTile(TargetTile) || !CanUnitBeOnTile(GridUnit, TargetTile)) { continue; }    	
			const int32 CalculatedMovement = Current.Cost + InterpretMovementCost(GridUnit, TargetTile);
			if (AvailableMovement < CalculatedMovement) { continue; }
    			
			FGridMovement Temp;
			Temp.GridTile = TargetTile;
			Temp.Cost = CalculatedMovement;
			Temp.ParentTile = Current.GridTile;

			if (!Processed.Contains(Temp))
			{
				ProcessingQueue.Enqueue(Temp);
			}
			else
			{
				FGridMovement* ProcessedCopy = Processed.Find(Temp);
				if (Temp.Cost < ProcessedCopy->Cost)
				{
					Processed.Remove(*ProcessedCopy);
					ProcessingQueue.Enqueue(Temp);
				}
			}	
		}		
	}
	
	OutMovement = Processed.Array();
}

void UGridPositionBasedInterpreterComponent::CalculateGridAttacks(TArray<FGridPair>& OutGridPairs,
	AGridUnitBase* GridUnit, const TArray<FGridMovement>& InGridMovements)
{
	if (!IsValid(GridUnit)) { return; }

	// get enemy info
	TArray<AGridUnitBase*> EnemyGridUnits;
	if (UTurnWorldSubsystem* TurnSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
	{
		TurnSubsystem->GetFactionEnemies(GridUnit, EnemyGridUnits);
	}
	TArray<FGridPosition> EnemyPositions;
	for (auto Unit : EnemyGridUnits)
	{
		EnemyPositions.Add(UGridMechanics_GridPositionLibrary::CalculateGridPositionFromSize(Unit, GridSize));
	}

	// unit info
	TMap<int32, TSet<AGridTileBase*>> RangeTileMap;
	
	TMap<FGridPosition, bool> MovementMap;
	for (const auto GridMovement : InGridMovements)
	{
		MovementMap.Add(GridTileLocationMap[GridMovement.GridTile], false);
	}

	// TODO: for now... need to figure out better way to gain access to weapon information
	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	if (!GameMode) { return; }
	
	TArray<FGridPosition> TempPositions;
	for (AGridUnitBase* EnemyUnit : EnemyGridUnits)
	{
		for (const int32 WeaponRange : GameMode->GetCombatCalculator()->GetWeaponRangesByName(GridUnit->GetWeaponsInMap()))
		{
			TempPositions.Empty();
			UGridMechanics_GridPositionLibrary::GetGridPositionsAtRange(
				UGridMechanics_GridPositionLibrary::CalculateGridPositionFromSize(EnemyUnit, GridSize), WeaponRange, TempPositions);
			for (FGridPosition RangePosition : TempPositions)
			{
				if (MovementMap.Contains(RangePosition) && MovementMap[RangePosition] == false)
				{
					MovementMap[RangePosition] = true;
					// find the tile that matches with this unit					
					OutGridPairs.Add(
						FGridPair(LocationGridTileMap[GridUnitLocationMap[EnemyUnit]], EnemyUnit));
				}
			}
		}		
	}
}

void UGridPositionBasedInterpreterComponent::CalculateGridAttackTiles(TMap<AGridTileBase*, int32>& OutWeaponPositions,
	const TArray<FGridMovement>& InGridMovements, const AGridUnitBase* InstigatorUnit, const AGridUnitBase* TargetUnit)
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }

	// TODO: this is a bit hacky...
	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	if (!GameMode) { return; }
	
	for (auto WeaponRange : GameMode->GetCombatCalculator()->GetWeaponRangesByName(InstigatorUnit->GetWeaponsInMap()))
	{
		if (WeaponRange == 0) { continue; }
		TArray<FGridPosition> OutWeaponRangePositions;
		UGridMechanics_GridPositionLibrary::GetGridPositionsAtRange(
			UGridMechanics_GridPositionLibrary::CalculateGridPositionFromSize(TargetUnit, GridSize), WeaponRange, OutWeaponRangePositions);
		for (const FGridPosition GridPosition : OutWeaponRangePositions)
		{			
			for (auto GridMovement : InGridMovements)
			{
				if (GridTileLocationMap[GridMovement.GridTile] == GridPosition)
				{
					OutWeaponPositions.Add(LocationGridTileMap[GridPosition], WeaponRange);
				}
			}
		}
	}
}
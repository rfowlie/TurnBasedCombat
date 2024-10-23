// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/GridManager.h"

#include "Grid/GridUtility.h"
#include "Grid/TurnManager.h"
#include "TurnBasedCombat/Public/Grid/Tile/GridTile.h"
#include "TurnBasedCombat/Public/Grid/Unit/GridUnit.h"


UGridManager::UGridManager()
{
	// TurnManager = NewObject<UTurnManager>(this);
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
}

void UGridManager::RegisterGridTile(AGridTile* GridTile)
{
	if (IsValid(GridTile) && !GridTilesAll.Contains(GridTile))
	{
		GridTilesAll.AddUnique(GridTile);
		
		// add to map
		FGridPosition GridPosition = UGridUtility::CalculateGridPosition(GridTile);
		LocationGridTileMap.Add(GridPosition, GridTile);
		
		// bind to events
		GridTile->OnBeginCursorOver.AddDynamic(this, &UGridManager::OnBeginCursorOverGridTile);
	}
}

void UGridManager::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (IsValid(GridUnit) && !GridUnitsAll.Contains(GridUnit))
	{
		GridUnitsAll.AddUnique(GridUnit);
		
		// add to map
		FGridPosition GridPosition = UGridUtility::CalculateGridPosition(GridUnit);
		
		// TODO: why the double map???
		LocationGridUnitMap.Add(GridPosition, GridUnit);
		GridUnitLocationMap.Add(GridUnit, GridPosition);
		
		// add to turn manager
		TurnManager->RegisterGridUnit(GridUnit);
	}
}

UGridProxy* UGridManager::GetCurrentHoveredGridTile()
{
	return NewObject<UGridProxy>(this);
}

UGridProxy* UGridManager::GetNextGridUnit(UGridProxy* InGridProxy, bool Next)
{
	// TODO: if given a proxy with a unit, return next or previous proxy with unit, otherwise return random
	return nullptr;
}

void UGridManager::OnBeginCursorOverGridTile(AActor* Actor)
{
	if (AGridTile* GridTile = Cast<AGridTile>(Actor); IsValid(GridTile))
	{
		// TODO: how do we turn this into a GridProxy???
		// use the grid tile to find the proxy that is in some list
		
		
		// tile
		if (IsValid(CurrentHoveredTile))
		{
			CurrentHoveredTile->SetHovered(false);
		}
		
		CurrentHoveredTile = GridTile;
		CurrentHoveredTile->SetHovered(true);
		if (OnGridTileHovered.IsBound())
		{
			// OnGridTileHovered.Broadcast();
			// OnGridTileHovered.Broadcast(GridTileHovered->TerrainDataAsset, GridTileHovered->GetSnapshot(), GridTileHovered->Terrain_Internal);
		}

		// unit
		if (IsValid(CurrentHoveredUnit))
		{
			CurrentHoveredUnit->SetHovered(false);
			CurrentHoveredUnit = nullptr;
		}

		// TODO: simplify this with better tile/unit setup or grid proxy
		// const auto GridPosition = CalculateGridPosition(GridTileHovered);
		// for (auto GridUnit : AllGridUnits)
		// {
		// 	if (GridPosition == CalculateGridPosition(GridUnit))
		// 	{
		// 		GridUnitHovered = GridUnit;
		// 		GridUnitHovered->SetHovered(true);
		// 		break;
		// 	}
		// }
		//
		// OnGridUnitHovered.Broadcast(GridUnitHovered, FUnitStatsSnapshot());
	}
}

TArray<FGridMovement> UGridManager::CalculateGridMovement(AGridUnit* GridUnit)
{
	// get start tile from terrain, make FStruct and set values, add Struct to queue
    TSet<FGridMovement> Processed;
    TSet<FGridMovement> ToProcess;
    TQueue<FGridMovement> ProcessingQueue;

	if (!GridUnitLocationMap.Contains(GridUnit))
	{
		// GridUnit does not have a location...
		return Processed.Array();
	}

    // const AGridTile* StartTile = *GridTileMap.Find(StartGridPosition);
    const AGridTile* StartTile = LocationGridTileMap[GridUnitLocationMap[GridUnit]];
    
    if (!IsValid(StartTile))
    {
    	// UE_LOG(LogTemp, Error, TEXT("Tile Actor At Location (X:%d, Y:%d) is null!!"), StartGridPosition.X, StartGridPosition.Y);
    	UE_LOG(LogTemp, Error, TEXT("Tile actor is null!!"));
    	return Processed.Array();
    }
    
    FGridMovement StartTileInfo;
	StartTileInfo.GridPosition = GridUnitLocationMap[GridUnit];
    StartTileInfo.AvailableMovement = GridUnit->GetAvailableMovement();
	StartTileInfo.GridTile = StartTile;
    	
    ProcessingQueue.Enqueue(StartTileInfo);

    FGridMovement Current;
    while(!ProcessingQueue.IsEmpty())
    {		
    	while (!ProcessingQueue.IsEmpty())
    	{		
    		ProcessingQueue.Dequeue(Current);
    		Processed.Add(Current);

    		// adjacent tiles
    		for (AGridTile* GridTile : GetGridTilesAtRange(Current.GridPosition, 1))
    		{
    			// check if available movement goes below 0, do not create info
    			// TODO: decide best best way to store terrain data and access it
    			const int32 CalculatedMovement =
    				Current.AvailableMovement - GridTile->TerrainDataAsset->TerrainStats.MovementCost;

    			if (CalculatedMovement < 0)
    			{
    				continue;
    			}
    			
    			// create filled struct
    			FGridMovement Temp;
    			Temp.GridTile = GridTile;
    			Temp.GridPosition = UGridUtility::CalculateGridPosition(GridTile);
    			// GetGridPosition_DEPRECATED(GridTile, Temp.GridPosition);
    			Temp.AvailableMovement = CalculatedMovement;

    			// check if struct processed
    			if (!Processed.Contains(Temp))
    			{
    				// check if struct in to process
    				if (FGridMovement* MatchingTerrain = ToProcess.Find(Temp))
    				{
    					if (MatchingTerrain->AvailableMovement < Temp. AvailableMovement)
    					{
    						MatchingTerrain->AvailableMovement = Temp.AvailableMovement;
    					}
    				}
    				else
    				{
    					ToProcess.Add(Temp);
    				}
    			}			
    		}		
    	}

    	// move all to process into queue
    	for (FGridMovement Info : ToProcess)
    	{
    		ProcessingQueue.Enqueue(Info);
    	}

    	// empty to process
    	ToProcess.Empty();		
    }

    // convert to array and sort according to gridlocation
    TArray<FGridMovement> Output = Processed.Array();
    Output.Sort([](const FGridMovement& A, const FGridMovement& B)
    {
    	return A.GridPosition.X < B.GridPosition.X ? true : A.GridPosition.Y < B.GridPosition.Y ? true : false;
    });
    
    return Output;
}

void UGridManager::CalculateGridAttacks(TArray<FGridPosition>& OutGridPositions, AGridUnit* GridUnit)
{
	TMap<int32, TSet<AGridTile*>> RangeTileMap;
	TSet<int32> WeaponRanges = GridUnit->GetWeaponRanges();
	TArray<FGridPosition> EnemyPositions = GetEnemyPositions(GridUnit);
	TArray<FGridMovement> GridMovements = CalculateGridMovement(GridUnit);
	TMap<FGridPosition, bool> MovementMap;
	for (const auto GridMovement : GridMovements)
	{
		MovementMap.Add(GridMovement.GridPosition, false);
	}

	TArray<FGridPosition> TempPositions;
	for (const FGridPosition EnemyPosition : EnemyPositions)
	{
		for (const int32 WeaponRange : WeaponRanges)
		{
			TempPositions.Empty();
			UGridUtility::GetGridPositionsAtRange(EnemyPosition, WeaponRange, TempPositions);
			for (FGridPosition RangePosition : TempPositions)
			{
				if (MovementMap.Contains(RangePosition))
				{
					MovementMap[RangePosition] = true;
				}
			}
		}		
	}

	// only return map values that are true
	for (auto Pair : MovementMap)
	{
		if (Pair.Value)
		{
			OutGridPositions.Add(Pair.Key);
		}
	}
}

TArray<FTargetingUnit> UGridManager::CalculateGridTargets(AGridUnit* GridUnit)
{
	// first round filter
		// find valid moveable tiles that can target enemies
		// for each valid tile, 




	//////////////////////////
	TArray<FTargetingUnit> UnitsBeingTargeted;
	
	TMap<int32, TSet<AGridTile*>> RangeTileMap;
	TSet<int32> WeaponRanges = GridUnit->GetWeaponRanges();
	TArray<FGridPosition> EnemyPositions = GetEnemyPositions(GridUnit);
	TArray<FGridMovement> GridMovements = CalculateGridMovement(GridUnit);
	TMap<FGridPosition, bool> MovementMap;
	// TMap<FGridPosition, TArray<int32>> SuccessMap;
	for (const auto GridMovement : GridMovements)
	{
		MovementMap.Add(GridMovement.GridPosition, false);
	}

	TArray<FGridPosition> TempPositions;
	for (const FGridPosition EnemyPosition : EnemyPositions)
	{
		for (const int32 WeaponRange : WeaponRanges)
		{
			UGridUtility::GetGridPositionsAtRange(EnemyPosition, WeaponRange, TempPositions);
			for (FGridPosition RangePosition : TempPositions)
			{
				if (MovementMap.Contains(RangePosition))
				{
					MovementMap[RangePosition] = true;
					// if (!SuccessMap.Contains(RangePosition))
					// {
					// 	SuccessMap.Add(RangePosition, TArray<int32>());						
					// }
					//
					// SuccessMap[RangePosition].AddUnique(WeaponRange);
				}
			}
		}
		
	}

	return TArray<FTargetingUnit>();



	///////////////////////////////////////////////////////////
	// GWS->GetUnitGridMovement(ActiveUnit, SelectedUnitGridMovement);

	// only test against tiles that have units not on the same team
	// auto GridTilesWithUnits = GWS->GetGridTilesWithUnitsByTeam(ActiveUnit, false);

	// for (auto GridTileWithUnit : GridTilesWithUnits)
	// {
	// 	FTargetingUnit TargetingUnit;
	// 	TargetingUnit.GridTile = GridTileWithUnit;
	// 	
	// 	auto TempGridPosition = GWS->CalculateGridPosition(GridTileWithUnit);
	// 	for (auto Range : WeaponRanges)
	// 	{
	// 		for (auto GridTileAtRange : GWS->GetGridTilesAtRange(TempGridPosition, Range))
	// 		{
	// 			for (auto UnitMovementTile : SelectedUnitGridMovement)
	// 			{
	// 				if (UnitMovementTile.GridTile == GridTileAtRange)
	// 				{
	// 					if (TargetingUnit.RangeMap.Contains(Range))
	// 					{
	// 						TargetingUnit.RangeMap[Range].AddUnique(UnitMovementTile.GridTile);
	// 					}
	// 					else
	// 					{
	// 						TargetingUnit.RangeMap.Add(Range, TArray<TSoftObjectPtr<AATBGridTile>>());
	// 						TargetingUnit.RangeMap[Range].AddUnique(UnitMovementTile.GridTile);
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	//
	// 	// only add if there are tiles in range
	// 	if (!TargetingUnit.RangeMap.IsEmpty())
	// 	{
	// 		UnitsBeingTargeted.Add(TargetingUnit);
	// 	}
	// }
	//
	// // highlight all attackable units
	// for (auto TargetingUnit : UnitsBeingTargeted)
	// {
	// 	TargetingUnit.GridTile->SetGridTileTagState(TAG_Grid_State_Attackable);
	// }
	//
	// return UnitsBeingTargeted;
}

TArray<FGridPosition> UGridManager::GetEnemyPositions(const AGridUnit* GridUnit) const
{
	TArray<FGridPosition> Positions;
	for (const auto Unit : GridUnitsAll)
	{
		if (GridUnit->GetFaction() != Unit->GetFaction())
		{
			Positions.AddUnique(UGridUtility::CalculateGridPosition(Unit));
		}		
	}
	
	return Positions;
}

TArray<AGridTile*> UGridManager::GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range)
{
	TArray<AGridTile*> Output;
	TArray<FGridPosition> Temp;
	UGridUtility::GetGridPositionsAtRange(StartGridPosition, Range, Temp);
	for (FGridPosition GridLocation : Temp)
	{
		if (LocationGridTileMap.Contains(GridLocation))
		{
			Output.Add(LocationGridTileMap[GridLocation]);
		}
	}

	return Output;
}

TArray<AGridTile*> UGridManager::GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges)
{
	// TODO: this look funny???
	TSet UniqueRanges(Ranges);
	for (const int32 Range : Ranges)
	{
		UniqueRanges.Add(FMath::Abs(Range));
	}

	TSet<AGridTile*> Output;
	for (const int32 Range : UniqueRanges)
	{
		Output.Append(GetGridTilesAtRange(StartGridPosition, Range));
	}

	return Output.Array();
}

TArray<AGridTile*> UGridManager::GetGridTilesInRange(const FGridPosition StartGridPosition, int32 Range)
{
	Range = FMath::Abs(Range);
	TSet<FGridPosition> GridLocationsSet;
	TArray<FGridPosition> NewGridPositions;

	TSet<AGridTile*> Output;
	for (int32 i = 1; i <= Range; i++)
	{
		Output.Append(GetGridTilesAtRange(StartGridPosition, i));
	}

	return Output.Array();
}

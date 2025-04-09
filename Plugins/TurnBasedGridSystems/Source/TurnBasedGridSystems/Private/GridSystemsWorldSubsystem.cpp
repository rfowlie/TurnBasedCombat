// Fill out your copyright notice in the Description page of Project Settings.

#include "GridSystemsWorldSubsystem.h"
#include "GridInfo.h"
#include "GridSystemsUtility.h"
#include "GridTileBase.h"
#include "GridUnitBase.h"
#include "ObjectPool.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


bool UGridSystemsWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// // do not create this subsystem if the game mode does not implement this interface
	// if (IGridInformationProvider* Interface = Cast<IGridInformationProvider>(GetWorld()->GetAuthGameMode()))
	// {
	// 	return true;
	// }
	//
	// UE_LOG(LogTemp, Error, TEXT("GameMode does not implement IGridInformationProvider, GridWorldSubsystem will not be created."))
	// return false;

	return true;
}

void UGridSystemsWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();

	// get information from gamemode
	// if (IGridInformationProvider* Interface = Cast<IGridInformationProvider>(GetWorld()->GetAuthGameMode()))
	// {
	// 	GridMap = NewObject<UGridMapBase>(Interface->GetGridMapClass());
	//
	// 	if (GridMap == nullptr)
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("GridMap is nullptr"));
	// 	}
	// }
}

void UGridSystemsWorldSubsystem::RegisterGridInfo(AGridInfo* InGridInfo)
{
	if (IsValid(InGridInfo) && GridInfo == nullptr)
	{
		GridInfo = InGridInfo;	
	}
}

void UGridSystemsWorldSubsystem::RegisterGridTile(AGridTileBase* GridTile)
{
	// if (IsValid(GridTile) && GridMap != nullptr)
	// {
	// 	GridMap->RegisterGridTile(GridTile);
	// 	GridTile->OnBeginCursorOver.AddDynamic(this, &ThisClass::OnBeginCursorOverGridTile);
	// }

	// TESTING
	if (IsValid(GridTile))
	{
		GridTile->OnBeginCursorOver.AddDynamic(this, &ThisClass::OnBeginCursorOverGridTile);
	}

	
	// if (IsValid(GridTile) && !GridTilesAll.Contains(GridTile))
	// {
	// 	GridTilesAll.AddUnique(GridTile);
	// 	
	// 	// add to map
	// 	const FGridPosition GridPosition = UGridHelper::CalculateGridPosition(GridInfo, GridTile);
	// 	LocationGridTileMap.Add(GridPosition, GridTile);
	// 	GridTileLocationMap.Add(GridTile, GridPosition);
	// 	
	// 	// bind to events
	// 	GridTile->OnBeginCursorOver.AddDynamic(this, &ThisClass::OnBeginCursorOverGridTile);
	//
	// 	UE_LOG(LogTemp, Log, TEXT("GridWorldSubsystem - RegisterGridTile: %s"), *GridTile->GetName());
	// }
}

// TODO: register will now forward items to the customizable grid manager
void UGridSystemsWorldSubsystem::RegisterGridUnit(AGridUnitBase* GridUnit)
{
	if (IsValid(GridUnit) && !GridUnitsAll.Contains(GridUnit))
	{
		GridUnitsAll.AddUnique(GridUnit);
		
		// add to map
		FGridPosition GridPosition = UGridSystemsUtility::CalculateGridPosition(GridInfo, GridUnit);
		
		LocationGridUnitMap.Add(GridPosition, GridUnit);
		GridUnitLocationMap.Add(GridUnit, GridPosition);
	}
}

void UGridSystemsWorldSubsystem::OnBeginCursorOverGridTile(AActor* Actor)
{
	if (AGridTileBase* GridTile = Cast<AGridTileBase>(Actor); IsValid(GridTile))
	{		
		// update tile
		if (IsValid(GridTileHovered))
		{
			if (OnGridTileHoveredStop.IsBound())
			{
				OnGridTileHoveredStop.Broadcast(GridTileHovered);
			}
			
			// GridTileHovered->SetHovered(false);
			GridTileHovered = nullptr;
		}

		if (bCanTileHover)
		{
			GridTileHovered = GridTile;
			// GridTileHovered->SetHovered(true);
			if (OnGridTileHoveredStart.IsBound())
			{
				OnGridTileHoveredStart.Broadcast(GridTileHovered);
			}
		}
		

		// update unit
		// if (IsValid(GridUnitHovered))
		// {
		// 	if (OnGridUnitHoveredStop.IsBound())
		// 	{
		// 		OnGridUnitHoveredStop.Broadcast(GridUnitHovered);
		// 	}
		// 	
		// 	GridUnitHovered->SetHovered(false);
		// 	GridUnitHovered = nullptr;			
		// }
		//
		// // TODO: need to separate but still know???
		// const FGridPosition GridPosition = UGridHelper::CalculateGridPosition(GridInfo, GridTileHovered);
		// if (LocationGridUnitMap.Contains(GridPosition))
		// {
		// 	GridUnitHovered = LocationGridUnitMap[GridPosition];
		// 	GridUnitHovered->SetHovered(true);
		// 	if (OnGridUnitHoveredStart.IsBound())
		// 	{
		// 		OnGridUnitHoveredStart.Broadcast(GridUnitHovered);
		// 	}
		// }
	}
}

void UGridSystemsWorldSubsystem::UpdateTileMapping(AGridTileBase* GridTile)
{
}

void UGridSystemsWorldSubsystem::UpdateTileMappingsAll()
{
}

void UGridSystemsWorldSubsystem::UpdateUnitMapping(AGridUnitBase* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }
	
	// remove mapping for grid unit
	LocationGridUnitMap.Remove(GridUnitLocationMap[GridUnit]);
	GridUnitLocationMap.Remove(GridUnit);

	// update the unit that has moved	
	const FGridPosition GridPosition = UGridSystemsUtility::CalculateGridPosition(GridInfo, GridUnit);
	LocationGridUnitMap.Add(GridPosition, GridUnit);
	GridUnitLocationMap.Add(GridUnit, GridPosition);
}

void UGridSystemsWorldSubsystem::UpdateUnitMappingsAll()
{
	for (AGridUnitBase* GridUnit : GridUnitsAll)
	{
		UpdateUnitMapping(GridUnit);
	}
}

AGridUnitBase* UGridSystemsWorldSubsystem::GetGridUnitOnTile(const AGridTileBase* GridTile) const
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

AGridTileBase* UGridSystemsWorldSubsystem::GetGridTileOfUnit(const AGridUnitBase* GridUnit) const
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

TArray<AGridTileBase*> UGridSystemsWorldSubsystem::GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range)
{
	TArray<AGridTileBase*> Output;
	TArray<FGridPosition> Temp;
	UGridSystemsUtility::GetManhattanDistance(StartGridPosition, Range, Temp);
	for (FGridPosition GridLocation : Temp)
	{
		if (LocationGridTileMap.Contains(GridLocation))
		{
			Output.Add(LocationGridTileMap[GridLocation]);
		}
	}

	return Output;
}

TArray<AGridTileBase*> UGridSystemsWorldSubsystem::GetGridTilesAtRanges(
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

void UGridSystemsWorldSubsystem::CalculateGridMovement(TArray<FGridMovement>& OutMovement, const AGridUnitBase* GridUnit, const int32 AvailableMovement)
{
	if (!GridUnitLocationMap.Contains(GridUnit))
	{
		// GridUnit does not have a location...
		UE_LOG(LogTemp, Error, TEXT("Grid unit does not have location!!"));
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
			if (GetGridUnitOnTile(TargetTile)) { continue; }    	
			const int32 CalculatedMovement = Current.Cost + TargetTile->GetMovementCost(); 
			// if (GridUnit->GetAvailableMovement() < CalculatedMovement)
			if (AvailableMovement < CalculatedMovement)
			{
				continue;
			}
    			
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

# pragma region cursor
TArray<AActor*> UGridSystemsWorldSubsystem::GetCursorByClass(TSubclassOf<AActor> CursorClass)
{
	TArray<AActor*> OutActors;
	if (!CursorClass) return OutActors;
	
	// if pool does not exist create
	if (!CursorPools.Contains(CursorClass))
	{
		if (UObjectPool* ObjectPool = UObjectPool::Create(GetWorld(), CursorClass, 1))
		{
			CursorPools.Add(CursorClass, ObjectPool);
		}
	}

	// pool creation failed
	if (!CursorPools.Contains(CursorClass)) { return OutActors; }
	
	OutActors = CursorPools[CursorClass]->GetObjects(1);
	return OutActors;
}

TArray<AActor*> UGridSystemsWorldSubsystem::GetCursorsByClass(const TSubclassOf<AActor> CursorClass, int32 Amount)
{
	TArray<AActor*> OutActors;
	if (!CursorClass) return OutActors;
	Amount = FMath::Max(Amount, 1);
	
	// if pool does not exist create
	if (!CursorPools.Contains(CursorClass))
	{
		if (UObjectPool* ObjectPool = UObjectPool::Create(GetWorld(), CursorClass, Amount))
		{
			CursorPools.Add(CursorClass, ObjectPool);
		}
	}

	// pool creation failed
	if (!CursorPools.Contains(CursorClass)) { return OutActors; }
	
	OutActors = CursorPools[CursorClass]->GetObjects(Amount);
	return OutActors;
}

# pragma endregion 

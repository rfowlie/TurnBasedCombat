// Fill out your copyright notice in the Description page of Project Settings.


#include "FlatSpaced/GridMapper_FlatEqualSpacing.h"
#include "GridHelper.h"
#include "GridTileBase.h"
#include "GridUnitBase.h"


void UGridMapper_FlatEqualSpacing::RegisterGridTile(AGridTileBase* InGridTile)
{
	if (!IsValid(InGridTile)) return;
	const FGridPosition GridPosition = CalculateGridPosition(InGridTile);
	LocationGridTileMap.Add(GridPosition, InGridTile);
	GridTileLocationMap.Add(InGridTile, GridPosition);
}

void UGridMapper_FlatEqualSpacing::RegisterGridTiles(const TArray<AGridTileBase*>& InGridTiles)
{
	for (AGridTileBase* GridTile : InGridTiles)
	{
		if (!IsValid(GridTile)) continue;
		const FGridPosition GridPosition = CalculateGridPosition(GridTile);
		LocationGridTileMap.Add(GridPosition, GridTile);
		GridTileLocationMap.Add(GridTile, GridPosition);
	}
}

FGridPosition UGridMapper_FlatEqualSpacing::CalculateGridPosition(const AActor* Actor)
{
	FGridPosition GridPosition;
	if (IsValid(Actor))
	{
		GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / GridSize);
		GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / GridSize);
	}	
	
	return GridPosition;
}

AGridTileBase* UGridMapper_FlatEqualSpacing::GetGridTileOfUnit(const AGridUnitBase* GridUnit) const
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

void UGridMapper_FlatEqualSpacing::RegisterGridUnit(AGridUnitBase* InGridUnit)
{
	if (!IsValid(InGridUnit)) return;
	const FGridPosition GridPosition = CalculateGridPosition(InGridUnit);
	LocationGridUnitMap.Add(GridPosition, InGridUnit);
	GridUnitLocationMap.Add(InGridUnit, GridPosition);
}

AGridUnitBase* UGridMapper_FlatEqualSpacing::GetGridUnitOnTile(const AGridTileBase* GridTile) const
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

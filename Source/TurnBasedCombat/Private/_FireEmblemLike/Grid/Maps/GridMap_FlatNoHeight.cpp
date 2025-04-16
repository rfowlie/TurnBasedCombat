// Fill out your copyright notice in the Description page of Project Settings.


#include "_FireEmblemLike/Grid/Maps/GridMap_FlatNoHeight.h"

#include "Grid/GridAlgo.h"
#include "Grid/GridStructs.h"


// FGridPosition UGridMap_FlatNoHeight::CalculateGridPosition(const AGridInfo* GridInfo, const AActor* Actor)
// {
// 	FGridPosition GridPosition;
// 	if (IsValid(Actor))
// 	{
// 		GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / GridInfo->TileSize);
// 		GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / GridInfo->TileSize);
// 	}	
// 	
// 	return GridPosition;
// }

FGridPosition UGridMap_FlatNoHeight::CalculateGridPosition(const AActor* Actor)
{
	FGridPosition GridPosition;
		if (IsValid(Actor))
		{
			GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / 200.f);
			GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / 200.f);
		}	
		
		return GridPosition;
}

void UGridMap_FlatNoHeight::CalculateGridAttackTiles(
	TMap<AGridTile*, int32>& OutWeaponPositions,
	const TArray<FGridMovement>& InGridMovements,
	const AGridUnit* InstigatorUnit,
	const TArray<int32>& WeaponRanges,
	const AGridUnit* TargetUnit)
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }
	
	for (auto WeaponRange : WeaponRanges)
	{
		if (WeaponRange == 0) { continue; }
		TArray<FGridPosition> OutWeaponRangePositions;
		UGridAlgo::GetManhattanDistance(CalculateGridPosition(TargetUnit), WeaponRange, OutWeaponRangePositions);
		for (const FGridPosition GridPosition : OutWeaponRangePositions)
		{			
			for (auto GridMovement : InGridMovements)
			{
				if (FGridPosition TempGridPosition = CalculateGridPosition(GridMovement.GridTile); TempGridPosition == GridPosition)
				{
					OutWeaponPositions.Add(GridMovement.GridTile, WeaponRange);
				}
			}
		}
	}	
}

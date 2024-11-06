// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridUtility.h"


int32 UGridUtility::GetDistanceBetweenGridPositions(const FGridPosition& positionA, const FGridPosition& positionB)
{
	return FMath::Abs(positionA.X - positionB.X) + FMath::Abs(positionA.Y - positionB.Y);
}

FGridPosition UGridUtility::CalculateGridPosition(const AActor* Actor)
{
	FGridPosition GridPosition;
	if (IsValid(Actor))
	{
		GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / GetGridSize());
		GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / GetGridSize());
	}	
	
	return GridPosition;
}

void UGridUtility::GetGridPositionsAtRange(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations)
{
	Range = FMath::Abs(Range);
	TSet<FGridPosition> Tiles;
	for (int32 i = -Range; i <= Range; i++)
	{
		// y = +-i - start.x
		auto Temp = FGridPosition(StartGridPosition.X + i, StartGridPosition.Y + FMath::Abs(i) - Range);
		auto Temp2 = FGridPosition(StartGridPosition.X + i, StartGridPosition.Y - (FMath::Abs(i) - Range));
		// Tiles.Add(FGridPosition(StartGridPosition.X + i, StartGridPosition.Y + ((Range + i) % Range)));
		// Tiles.Add(FGridPosition(StartGridPosition.X + i, StartGridPosition.Y - ((Range + i) % Range)));
		Tiles.Add(FGridPosition(StartGridPosition.X + i, StartGridPosition.Y + FMath::Abs(i) - Range));
		Tiles.Add(FGridPosition(StartGridPosition.X + i, StartGridPosition.Y - (FMath::Abs(i) - Range)));
	}

	GridLocations = Tiles.Array();
}

void UGridUtility::GetGridPositionsAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges,
	TArray<FGridPosition>& GridLocations)
{
	TSet UniqueRanges(Ranges);
	for (const int32 Range : Ranges)
	{
		UniqueRanges.Add(FMath::Abs(Range));
	}
	
	TSet<FGridPosition> GridLocationsSet;
	TArray<FGridPosition> TempGridLocations;
	for (const int32 Range : UniqueRanges)
	{
		GetGridPositionsAtRange(StartGridPosition, Range, TempGridLocations);
		GridLocationsSet.Append(TempGridLocations);
		TempGridLocations.Empty();
	}

	GridLocations = GridLocationsSet.Array();
}

void UGridUtility::GetGridPositionsInRange(const FGridPosition StartGridPosition, int32 Range,
                                           TArray<FGridPosition>& GridLocations)
{
	Range = FMath::Abs(Range);
	TSet<FGridPosition> GridLocationsSet;
	TArray<FGridPosition> NewGridPositions;

	// start at 1 no point in doing zero
    for (int32 i = 1; i <= Range; i++)
    {
    	GetGridPositionsAtRange(StartGridPosition, i, NewGridPositions);
        GridLocationsSet.Append(NewGridPositions);
    	NewGridPositions.Empty();
    }

	GridLocations = GridLocationsSet.Array();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMechanics_GridPositionLibrary.h"

#include "GridMechanics_Structs.h"

FGridPosition UGridMechanics_GridPositionLibrary::CalculateGridPositionFromSize(const AActor* Actor, int32 Size)
{
	Size = FMath::Max(10, Size);
	FGridPosition GridPosition;
	if (IsValid(Actor))
	{
		GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / Size);
		GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / Size);
	}
	
	return GridPosition;
}

void UGridMechanics_GridPositionLibrary::GetGridPositionsAtRange(const FGridPosition StartGridPosition, int32 Range,
	TArray<FGridPosition>& GridLocations)
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

void UGridMechanics_GridPositionLibrary::GetGridPositionsAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges,
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

void UGridMechanics_GridPositionLibrary::GetGridPositionsInRange(const FGridPosition StartGridPosition, int32 Range,
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

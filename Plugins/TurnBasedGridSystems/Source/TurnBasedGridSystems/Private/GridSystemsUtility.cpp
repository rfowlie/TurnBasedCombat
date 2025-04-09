// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystemsUtility.h"
#include "GridInfo.h"


// TODO: this function is only temporary and for helping with the refactor
FGridPosition UGridSystemsUtility::CalculateGridPosition(const AActor* Actor)
{
	FGridPosition GridPosition;
	if (IsValid(Actor))
	{
		GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / 200.f);
		GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / 200.f);
	}	
	
	return GridPosition;
}

FGridPosition UGridSystemsUtility::CalculateGridPositionWithInfo(const AGridInfo* GridInfo, const AActor* Actor)
{
	FGridPosition GridPosition;
	if (IsValid(Actor))
	{
		GridPosition.X = FMath::TruncToInt(Actor->GetActorLocation().X / GridInfo->TileSize);
		GridPosition.Y = FMath::TruncToInt(Actor->GetActorLocation().Y / GridInfo->TileSize);
	}	
	
	return GridPosition;
}

void UGridSystemsUtility::GetManhattanDistance(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations)
{
	TSet<FGridPosition> Tiles;
	Range = FMath::Abs(Range);

	for (int32 DX = -Range; DX <= Range; DX++)
	{
		int32 DY = Range - FMath::Abs(DX); // Compute the vertical range
		Tiles.Add({ StartGridPosition.X + DX, StartGridPosition.Y + DY });
		Tiles.Add({ StartGridPosition.X + DX, StartGridPosition.Y - DY });
	}

	GridLocations = Tiles.Array();
}

void UGridSystemsUtility::GetManhattanDistanceAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges,
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
		GetManhattanDistance(StartGridPosition, Range, TempGridLocations);
		GridLocationsSet.Append(TempGridLocations);
		TempGridLocations.Empty();
	}

	GridLocations = GridLocationsSet.Array();
}

void UGridSystemsUtility::GetChebyshevDistance(const FGridPosition& StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations)
{
	GridLocations.Empty(); // Clear the array before populating it.

	for (int32 dx = -Range; dx <= Range; dx++)
	{
		for (int32 dy = -Range; dy <= Range; dy++)
		{
			if (FMath::Max(FMath::Abs(dx), FMath::Abs(dy)) <= Range) // Chebyshev distance condition
			{
				GridLocations.Add(FGridPosition(StartGridPosition.X + dx, StartGridPosition.Y + dy));
			}
		}
	}
}

void UGridSystemsUtility::GetChebyshevDistanceAtRanges(
	const FGridPosition& StartGridPosition, TArray<int32> Ranges, TArray<FGridPosition>& GridLocations)
{
	TArray<int32> UniqueRanges;
	for (const int32 Range : Ranges)
	{
		UniqueRanges.Add(FMath::Abs(Range));
	}

	TSet<FGridPosition> GridLocationsSet;
	TArray<FGridPosition> TempGridLocations;
	for (const int32 Range : UniqueRanges)
	{
		GetChebyshevDistance(StartGridPosition, Range, TempGridLocations);
		GridLocationsSet.Append(TempGridLocations);
		TempGridLocations.Empty();
	}

	GridLocations = GridLocationsSet.Array();
}

void UGridSystemsUtility::GetGridPositionsInRange(const FGridPosition StartGridPosition, int32 Range,
                                          TArray<FGridPosition>& GridLocations)
{
	Range = FMath::Abs(Range);
	TSet<FGridPosition> GridLocationsSet;
	TArray<FGridPosition> NewGridPositions;

	// start at 1 no point in doing zero
    for (int32 i = 1; i <= Range; i++)
    {
    	GetManhattanDistance(StartGridPosition, i, NewGridPositions);
        GridLocationsSet.Append(NewGridPositions);
    	NewGridPositions.Empty();
    }

	GridLocations = GridLocationsSet.Array();
}

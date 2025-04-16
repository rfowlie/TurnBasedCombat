// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridAlgo.h"
#include "Grid/GridStructs.h"


void UGridAlgo::GetManhattanDistance(const FGridPosition StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations)
{
	Range = FMath::Abs(Range);

	for (int32 DX = -Range; DX <= Range; DX++)
	{
		const int32 DY = Range - FMath::Abs(DX);
		GridLocations.AddUnique({ StartGridPosition.X + DX, StartGridPosition.Y + DY });
		GridLocations.AddUnique({ StartGridPosition.X + DX, StartGridPosition.Y - DY });
	}
}

void UGridAlgo::GetManhattanDistanceAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges,
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

void UGridAlgo::GetChebyshevDistance(const FGridPosition& StartGridPosition, int32 Range, TArray<FGridPosition>& GridLocations)
{
	for (int32 DX = -Range; DX <= Range; DX++)
	{
		for (int32 DY = -Range; DY <= Range; DY++)
		{
			if (FMath::Max(FMath::Abs(DX), FMath::Abs(DY)) <= Range) // Chebyshev distance condition
			{
				GridLocations.AddUnique(FGridPosition(StartGridPosition.X + DX, StartGridPosition.Y + DY));
			}
		}
	}
}

void UGridAlgo::GetChebyshevDistanceAtRanges(
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

void UGridAlgo::CalculateGridMovement(TArray<FGridMovement>& OutMovement, const AGridUnit* GridUnit)
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "_Basic/Grid/GridShapeUtility.h"
#include "Grid/GridStructs.h"

// get all possible configurations as if (0,0) will be the shape that is checked against
void UGridShapeUtility::GetConfigurationConnectFour(
	TArray<FShapeConfiguration>& Configurations, const bool bRow, const bool bCol, const bool bDiagonal)
{
	// Configurations.Empty();
	
	if (bRow)
	{
		Configurations.Add(TArray { FGridPosition(-3,0), FGridPosition(-2,0), FGridPosition(-1,0), FGridPosition(0,0)});
		Configurations.Add(TArray { FGridPosition(-2,0), FGridPosition(-1,0), FGridPosition(0,0), FGridPosition(1,0)});
		Configurations.Add(TArray { FGridPosition(-1,0), FGridPosition(0,0), FGridPosition(1,0), FGridPosition(2,0)});
		Configurations.Add(TArray { FGridPosition(0,0), FGridPosition(1,0), FGridPosition(2,0), FGridPosition(3,0)});
	}
	if (bCol)
	{
		Configurations.Add(TArray { FGridPosition(0,-3), FGridPosition(0,-2), FGridPosition(0,-1), FGridPosition(0,0)});
		Configurations.Add(TArray { FGridPosition(0,-2), FGridPosition(0,-1), FGridPosition(0,0), FGridPosition(0,1)});
		Configurations.Add(TArray { FGridPosition(0,-1), FGridPosition(0,0), FGridPosition(0,1), FGridPosition(0,2)});
		Configurations.Add(TArray { FGridPosition(0,0), FGridPosition(0,1), FGridPosition(0,2), FGridPosition(0,3)});
	}
	if (bDiagonal)
	{
		Configurations.Add(TArray { FGridPosition(-3,-3), FGridPosition(-2,-2), FGridPosition(-1,-1), FGridPosition(0,0)});
		Configurations.Add(TArray { FGridPosition(-2,-2), FGridPosition(-1,-1), FGridPosition(0,0), FGridPosition(1,1)});
		Configurations.Add(TArray { FGridPosition(-1,-1), FGridPosition(0,0), FGridPosition(1,1), FGridPosition(2,2)});
		Configurations.Add(TArray { FGridPosition(0,0), FGridPosition(1,1), FGridPosition(2,2), FGridPosition(3,3)});

		Configurations.Add(TArray { FGridPosition(3,-3), FGridPosition(2,-2), FGridPosition(1,-1), FGridPosition(0,0)});
		Configurations.Add(TArray { FGridPosition(2,-2), FGridPosition(1,-1), FGridPosition(0,0), FGridPosition(-1,1)});
		Configurations.Add(TArray { FGridPosition(1,-1), FGridPosition(0,0), FGridPosition(-1,1), FGridPosition(-2,2)});
		Configurations.Add(TArray { FGridPosition(0,0), FGridPosition(-1,1), FGridPosition(-2,2), FGridPosition(-3,3)});
	}
}

void UGridShapeUtility::CheckConfiguration(
	const FGridPosition StartingPosition,
	const TArray<FGridPosition>& GridPositions,
	const TArray<FShapeConfiguration>& ConfigurationsToCheck,
	TArray<FShapeConfiguration>& OutSuccessfulConfigurations)
{
	if (!GridPositions.Contains(StartingPosition)) { return; }
	TMap<FGridPosition, bool> GridMap;
	for (auto GridPosition : GridPositions) { GridMap.Add(GridPosition, true); }
	
	for (auto Configuration : ConfigurationsToCheck)
	{
		bool bFind = true;
		for (auto Position : Configuration.ShapeConfiguration)
		{
			if (!GridMap.Contains(StartingPosition + Position))
			{
				bFind = false;
				break;
			}
		}

		if (bFind)
		{
			OutSuccessfulConfigurations.Add(Configuration);
		}
	}
}

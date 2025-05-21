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

void UGridShapeUtility::GetShapeConfiguration_Line(TArray<FShapeConfiguration>& Configurations,
	const EGridDirection GridDirection, int32 Length)
{
	if (Length < 2) { return; }
	if (Length > 10)
	{
		UE_LOG(LogTemp, Warning, TEXT("Excessive Value: %d, could cause issues so clamping..."), Length);
		Length = FMath::Clamp(Length, 2, 10);
	}

	switch (GridDirection)
	{
	case Row:
		for (int32 StartPos = 1 - Length; StartPos < 1; StartPos++)
		{
			FShapeConfiguration TempConfiguration;
			for (int32 CurrentPos = 0; CurrentPos < Length; CurrentPos++)
			{
				TempConfiguration.ShapeConfiguration.Add(FGridPosition(0, StartPos + CurrentPos));
			}
			Configurations.Add(TempConfiguration);
		}
		break;
	case Column:
		for (int32 StartPos = 1 - Length; StartPos < 1; StartPos++)
		{
			FShapeConfiguration TempConfiguration;
			for (int32 CurrentPos = 0; CurrentPos < Length; CurrentPos++)
			{
				TempConfiguration.ShapeConfiguration.Add(FGridPosition(StartPos + CurrentPos, 0));
			}
			Configurations.Add(TempConfiguration);
		}
		break;
	case Diagonal_TopDown:
		// high to low
		for (int32 StartPos = 1 - Length; StartPos < 1; StartPos++)
		{
			FShapeConfiguration TempConfiguration;
			for (int32 CurrentPos = 0; CurrentPos < Length; CurrentPos++)
			{
				TempConfiguration.ShapeConfiguration.Add(FGridPosition(StartPos + CurrentPos, StartPos + CurrentPos));
			}
			Configurations.Add(TempConfiguration);
		}
		break;
	case Diagonal_BottomUp:
		// low to high
		for (int32 StartPos = 1 - Length; StartPos < 1; StartPos++)
		{
			FShapeConfiguration TempConfiguration;
			for (int32 CurrentPos = 0; CurrentPos < Length; CurrentPos++)
			{
				TempConfiguration.ShapeConfiguration.Add(FGridPosition(StartPos + CurrentPos, -(StartPos + CurrentPos)));
			}
			Configurations.Add(TempConfiguration);
		}
		break;
	}
}

void UGridShapeUtility::GetLongestConnect(
	TArray<FGridPosition>& LongestConnect, const EGridDirection GridDirection,
	const FGridPosition& StartingPosition, const TArray<FGridPosition>& GridPositions,
	int32 Min, int32 Max)
{
	if (Min < 2 || Max > 10 || Max < Min)
	{
		UE_LOG(LogTemp, Error, TEXT("Min and Max values are invalid... (Min:%d) (Max:%d)"), Min, Max);
		return;
	}
	
	if (!GridPositions.Contains(StartingPosition)) { return; }
	TMap<FGridPosition, bool> GridMap;
	for (auto GridPosition : GridPositions) { GridMap.Add(GridPosition, false); }

	int32 RowDirectionMultiplier = 0;
	int32 ColumnDirectionMultiplier = 0;
	
	switch (GridDirection){
	case Row:
		ColumnDirectionMultiplier = 1;
		break;
	case Column:
		RowDirectionMultiplier = 1;
		break;
	case Diagonal_TopDown:
		RowDirectionMultiplier = 1;
		ColumnDirectionMultiplier = 1;
		break;
	case Diagonal_BottomUp:
		RowDirectionMultiplier = -1;
		ColumnDirectionMultiplier = 1;
		break;
	}
	
	for (int32 Index = 1 - Max; Index < Max; Index++)
	{
		FGridPosition TempPosition (
			Index * RowDirectionMultiplier, Index * ColumnDirectionMultiplier);
		if (GridMap.Contains(StartingPosition + TempPosition))
		{
			LongestConnect.Add(TempPosition);
		}
		else
		{
			if (Index > 0)
			{
				break;
			}
			
			LongestConnect.Empty();
		}
	}

	if (LongestConnect.Num() < Min) { LongestConnect.Empty(); }
}

void UGridShapeUtility::GetLongestLine(
	TArray<FGridPosition>& OutGridPositions, const TArray<FGridPosition>& InGridPositions,
	const FGridPosition& StartingPosition, const EGridDirection GridDirection)
{
	// if (!InGridPositions.Contains(StartingPosition)) { return; }
	TMap<FGridPosition, bool> GridMap;
	for (auto GridPosition : InGridPositions) { GridMap.Add(GridPosition, false); }

	int32 RowDirectionMultiplier = 0;
	int32 ColumnDirectionMultiplier = 0;
	
	switch (GridDirection){
	case Row:
		ColumnDirectionMultiplier = 1;
		break;
	case Column:
		RowDirectionMultiplier = 1;
		break;
	case Diagonal_TopDown:
		RowDirectionMultiplier = 1;
		ColumnDirectionMultiplier = 1;
		break;
	case Diagonal_BottomUp:
		RowDirectionMultiplier = -1;
		ColumnDirectionMultiplier = 1;
		break;
	}
	
	int32 Positive = 0;
	for (int32 Index = 1; Index <= MAX_int32; Index++)
	{
		FGridPosition TempPosition (
			Index * RowDirectionMultiplier, Index * ColumnDirectionMultiplier);
		if (!GridMap.Contains(StartingPosition + TempPosition)) { break; }
		Positive = Index;
	}
	
	int32 Negative = 0;
	for (int32 Index = 1; Index <= MAX_int32; Index++)
	{
		FGridPosition TempPosition (
			-Index * RowDirectionMultiplier, -Index * ColumnDirectionMultiplier);
		if (!GridMap.Contains(StartingPosition + TempPosition)) { break; }
		Negative = -Index;
	}

	for (int32 Index = Negative; Index <= Positive; Index++)
	{
		OutGridPositions.Add(
			StartingPosition +
			FGridPosition(Index * RowDirectionMultiplier, Index * ColumnDirectionMultiplier));
	}	
}

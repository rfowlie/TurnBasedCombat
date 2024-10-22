// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TurnBasedCombat/Public/Stats/Terrain/TerrainDataAsset.h"
#include "GridStructs.generated.h"


class UWeapon;
class AGridTile;


USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FGridPosition
{
	GENERATED_BODY()

	FGridPosition() : X(0), Y(0) {}
	FGridPosition(const int32 ValX, const int32 ValY) : X(ValX), Y(ValY) {}
	
	UPROPERTY(BlueprintReadOnly)
	int32 X;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Y;

	bool operator==(const FGridPosition& Other) const
	{
		return (X == Other.X) && (Y == Other.Y);
	}
	
	bool operator<(const FGridPosition& Other) const
	{
		return (X < Other.X) ? true : (Y < Other.Y) ? true : false;
	}

	// bool operator>(const FGridLocation& Other) const
	// {
	// 	return (X > Other.X) ? true : (Y > Other.Y) ? true : false;
	// }

	FGridPosition operator+(const FGridPosition& Other) const
	{
		return FGridPosition(X + Other.X, Y + Other.Y);
	}

	FGridPosition operator-(const FGridPosition& Other) const
	{
		return FGridPosition(X - Other.X, Y - Other.Y);
	}	
};

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridPosition& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return HashCombineFast(GetTypeHash(Struct.X), GetTypeHash(Struct.Y));
}

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FGridMovement
{
	GENERATED_BODY()

	FGridMovement() : GridPosition(FGridPosition()), AvailableMovement(0) {}
	// FGridMovement() : GridTile(nullptr), GridLocation(FGridPosition()), AvailableMovement(0) {}
	// FGridMovement(const FGridPosition GridPositionVal, const int32 AvailableMovementVal) : GridTile(nullptr),
	// 	GridLocation(GridPositionVal), AvailableMovement(AvailableMovementVal) {}
	// FGridMovement(AATBGridTile* GridTileVal, const FGridPosition GridPositionVal, const int32 AvailableMovementVal) :
	// 	GridTile(GridTileVal), GridLocation(GridPositionVal), AvailableMovement(AvailableMovementVal) {}
	

	UPROPERTY()
	TSoftObjectPtr<AGridTile> GridTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridPosition GridPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AvailableMovement;
	
	bool operator==(const FGridMovement& Other) const
	{
		return (GridPosition == Other.GridPosition);
	}
	
	bool operator<(const FGridMovement& Other) const
	{
		if (GridPosition.X < Other.GridPosition.X)
		{
			return true;
		}
		if (GridPosition.Y < Other.GridPosition.Y)
		{
			return true;
		}
		
		return false;
	}
};

USTRUCT()
struct FTargetingUnit
{
	GENERATED_BODY()

	UPROPERTY()
	AGridTile* GridTile = nullptr;

	UPROPERTY()
	AGridUnit* GridUnit = nullptr;

	UPROPERTY()
	TArray<UWeapon*> Weapons;
	
	TMap<int32, TArray<TSoftObjectPtr<AGridUnit>>> RangeMap;	
};

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridMovement& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return GetTypeHash(Struct.GridPosition);
}

USTRUCT(Blueprintable, BlueprintType)
struct FTileStatsSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTerrainStats TerrainStats;
	
};

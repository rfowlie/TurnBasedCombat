// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TurnBasedCombat/Public/Stats/Terrain/TerrainDataAsset.h"
#include "GridStructs.generated.h"


class AGridUnit;
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

	/*
	 * LEARNING
	 * you can have C++ functions on structs
	 */
	// int32 InRange(const FGridPosition& Other) const
	// {
	// 	return FMath::Abs((X - Other.X) + (Y - Other.Y));
	// }
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

	FGridMovement() {}
	// FGridMovement() : GridPosition(FGridPosition()), AvailableMovement(0) {}
	// FGridMovement() : GridTile(nullptr), GridLocation(FGridPosition()), AvailableMovement(0) {}
	// FGridMovement(const FGridPosition GridPositionVal, const int32 AvailableMovementVal) : GridTile(nullptr),
	// 	GridLocation(GridPositionVal), AvailableMovement(AvailableMovementVal) {}
	// FGridMovement(AATBGridTile* GridTileVal, const FGridPosition GridPositionVal, const int32 AvailableMovementVal) :
	// 	GridTile(GridTileVal), GridLocation(GridPositionVal), AvailableMovement(AvailableMovementVal) {}
	

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTile> GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTile> ParentTile = nullptr;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// FGridPosition GridPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 0;

	// UPROPERTY(BlueprintReadOnly)
	// FGridMovement& ParentNode;
	
	
	bool operator==(const FGridMovement& Other) const
	{
		// return (GridPosition == Other.GridPosition) && GridTile == Other.GridTile;
		return GridTile == Other.GridTile;
	}
	
	// bool operator<(const FGridMovement& Other) const
	// {
	// 	if (GridPosition.X < Other.GridPosition.X)
	// 	{
	// 		return true;
	// 	}
	// 	if (GridPosition.Y < Other.GridPosition.Y)
	// 	{
	// 		return true;
	// 	}
	// 	
	// 	return false;
	// }
};

USTRUCT(Blueprintable, BlueprintType)
struct FTargetingUnit
{
	GENERATED_BODY()

	UPROPERTY()
	AGridTile* GridTile = nullptr;

	UPROPERTY()
	AGridUnit* GridUnit = nullptr;
	
	UPROPERTY()
	TArray<UWeapon*> Weapons;
	
	TMap<int32, TArray<AGridUnit*>> RangeMap;
};

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridMovement& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return GetTypeHash(Struct.GridTile);
}

USTRUCT(Blueprintable, BlueprintType)
struct FTileStatsSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTerrainStats TerrainStats;
	
};

USTRUCT(BlueprintType)
struct FGridPair
{
	GENERATED_BODY()

	/*
	 * LEARNING
	 * Default constructors are mandatory for USTRUCTS???
	 */
	FGridPair() {}
	FGridPair(AGridTile* InGridTile, AGridUnit* InGridUnit) : GridTile(InGridTile), GridUnit(InGridUnit) {}
	
	UPROPERTY(BlueprintReadOnly)
	AGridTile* GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AGridUnit* GridUnit = nullptr;
	
};

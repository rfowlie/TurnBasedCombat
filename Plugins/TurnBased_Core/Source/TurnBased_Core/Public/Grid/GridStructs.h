// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Unit/GridUnit.h"
#include "GridStructs.generated.h"

class AGridUnit;
class AGridTile;
class UWeapon;

USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FGridPosition
{
	GENERATED_BODY()

	FGridPosition() : X(0), Y(0) {}
	FGridPosition(const int32 ValX, const int32 ValY) : X(ValX), Y(ValY) {}
	
	UPROPERTY(BlueprintReadOnly)
	int32 X;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Y;

	// FOR NOW
	int32 GetDistance(const FGridPosition& Other) const
	{
		// Chebyshev distance
		return FMath::Max(FMath::Abs(Other.X - X), FMath::Abs(Other.Y - Y));
	}
	
	bool operator==(const FGridPosition& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
	
	bool operator<(const FGridPosition& Other) const
	{
		return (X < Other.X) ? true : (Y < Other.Y) ? true : false;
	}

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
struct TURNBASED_CORE_API FGridMovement
{
	GENERATED_BODY()

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

// USTRUCT(Blueprintable, BlueprintType)
// struct TURNBASED_CORE_API FTargetingUnit
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	AGridTile* GridTile = nullptr;
//
// 	UPROPERTY()
// 	AGridUnit* GridUnit = nullptr;
// 	
// 	UPROPERTY()
// 	TArray<UWeapon*> Weapons;
// 	
// 	TMap<int32, TArray<AGridUnit*>> RangeMap;
// };

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridMovement& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return GetTypeHash(Struct.GridTile);
}

// USTRUCT(Blueprintable, BlueprintType)
// struct FTileStatsSnapshot
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	FTerrainStats TerrainStats;
// 	
// };

USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FGridPair
{
	GENERATED_BODY()
	
	FGridPair() {}
	FGridPair(AGridTile* InGridTile, AGridUnit* InGridUnit) : GridTile(InGridTile), GridUnit(InGridUnit) {}
	
	UPROPERTY(BlueprintReadOnly)
	AGridTile* GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AGridUnit* GridUnit = nullptr;
	
};

USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FFactionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Tag;
	
	UPROPERTY()
	TMap<AGridUnit*, bool> GridUnits;

	void GetGridUnits(TArray<AGridUnit*>& OutGridUnits)
	{
		GridUnits.GetKeys(OutGridUnits);
	}

	void ActivateUnits()
	{
		TArray<AGridUnit*> Units;
		GridUnits.GetKeys(Units);
		for (const AGridUnit* GridUnit : Units)
		{
			if (IsValid(GridUnit); GridUnit->GetHealth() > 0)
			{
				GridUnits[GridUnit] = true;
			}
		}
	}

	int32 GetActiveUnitsCount()
	{
		int32 Count = 0;
		for (const auto Pair : GridUnits)
		{
			// if pointer not null and value not false
			if (IsValid(Pair.Key); Pair.Value) { Count++; }
		}
		return Count;
	}
	
	bool IsFactionDefeated() const
	{
		TArray<AGridUnit*> Units;
		GridUnits.GetKeys(Units);
		for (const AGridUnit* GridUnit : Units)
		{
			if (IsValid(GridUnit); GridUnit->GetHealth() > 0)
			{
				return false;
			}
		}

		return true;
	}

	bool CanUnitsMove()
	{
		for (const auto Pair : GridUnits)
		{
			if (Pair.Value) { return true; }
		}

		return false;
	}

	AGridUnit* GetNextUnit(AGridUnit* InGridUnit) const
	{
		if (!GridUnits.Contains(InGridUnit)) { return InGridUnit; }
		TArray<AGridUnit*> Keys;
		GridUnits.GetKeys(Keys);
		int32 Index = Keys.Find(InGridUnit);
		Index = (Index + 1 + GridUnits.Num()) % GridUnits.Num();
		return Keys[Index];
	}
};

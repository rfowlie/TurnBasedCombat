// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GridUnitBase.h"
#include "UObject/Object.h"
#include "GridStructs.generated.h"

class AGridUnitBase;
class AGridTileBase;
class UWeapon;

USTRUCT(BlueprintType)
struct TURNBASEDGRIDSYSTEMS_API FGridPosition
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
struct TURNBASEDGRIDSYSTEMS_API FGridMovement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTileBase> GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTileBase> ParentTile = nullptr;
	
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
};

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridMovement& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return GetTypeHash(Struct.GridTile);
}

USTRUCT(BlueprintType)
struct TURNBASEDGRIDSYSTEMS_API FGridPair
{
	GENERATED_BODY()
	
	FGridPair() {}
	FGridPair(AGridTileBase* InGridTile, AGridUnitBase* InGridUnit) : GridTile(InGridTile), GridUnit(InGridUnit) {}
	
	UPROPERTY(BlueprintReadOnly)
	AGridTileBase* GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AGridUnitBase* GridUnit = nullptr;
	
};

USTRUCT(BlueprintType)
struct TURNBASEDGRIDSYSTEMS_API FFactionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Tag;
	
	UPROPERTY()
	TMap<AGridUnitBase*, bool> GridUnits;

	TArray<AGridUnitBase*> GetGridUnits() const
	{
		TArray<AGridUnitBase*> OutGridUnits;
		GridUnits.GetKeys(OutGridUnits);
		return OutGridUnits;
	}
	
	TArray<AGridUnitBase*> GetAliveGridUnits() const
	{		
		TArray<AGridUnitBase*> OutGridUnits;
		for (auto Pair : GridUnits)
		{
			// if (Pair.Key->GetHealth() > 0)
			// {
			// 	OutGridUnits.Add(Pair.Key);
			// }
		}
		return OutGridUnits;
	}

	void ActivateUnits()
	{
		TArray<AGridUnitBase*> Units;
		GridUnits.GetKeys(Units);
		for (const AGridUnitBase* GridUnit : Units)
		{
			// if (IsValid(GridUnit); GridUnit->GetHealth() > 0)
			// {
			// 	GridUnits[GridUnit] = true;
			// }
		}
	}

	void DeactivateUnits()
	{
		TArray<AGridUnitBase*> Units;
		GridUnits.GetKeys(Units);
		for (const AGridUnitBase* GridUnit : Units)
		{
			if (IsValid(GridUnit))
			{
				GridUnits[GridUnit] = false;
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
		TArray<AGridUnitBase*> Units;
		GridUnits.GetKeys(Units);
		for (const AGridUnitBase* GridUnit : Units)
		{
			// if (IsValid(GridUnit); GridUnit->GetHealth() > 0)
			// {
			// 	return false;
			// }
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

	AGridUnitBase* GetNextUnit(AGridUnitBase* InGridUnit) const
	{
		if (!GridUnits.Contains(InGridUnit)) { return InGridUnit; }
		TArray<AGridUnitBase*> Keys;
		GridUnits.GetKeys(Keys);
		int32 Index = Keys.Find(InGridUnit);
		Index = (Index + 1 + GridUnits.Num()) % GridUnits.Num();
		return Keys[Index];
	}
};

/*
 * calculate a score for each potential combat a unit can do on its turn
 */
USTRUCT()
struct FCombatScore
{
	GENERATED_BODY()

	UPROPERTY()
	AGridUnitBase* InstigatorUnit = nullptr;

	UPROPERTY()
	AGridTileBase* InstigatorTile = nullptr;

	UPROPERTY()
	FName InstigatorWeapon;
	
	UPROPERTY()
	AGridUnitBase* TargetUnit = nullptr;

	UPROPERTY()
	AGridTileBase* TargetTile = nullptr;

	UPROPERTY()
	FName TargetWeapon;
	
	// UPROPERTY()
	// FCombatSnapshot_Outcome CombatOutcome;
	//
	// UPROPERTY()
	// FCombatSnapshot_Basic InstigatorSnapshotBasic;
	//
	// UPROPERTY()
	// FCombatSnapshot_Advanced InstigatorSnapShotAdvanced;
	//
	// UPROPERTY()
	// FCombatSnapshot_Basic TargetSnapshotBasic;
	//
	// UPROPERTY()
	// FCombatSnapshot_Advanced TargetSnapShotAdvanced;


	// evaluate
	UPROPERTY()
	float Score = 0.f;

	void CalculateScore()
	{
		// Score = 0;
		// Score += InstigatorSnapShotAdvanced.HitChance / 100.f;
		// Score += InstigatorSnapShotAdvanced.CriticalChance / 100.f * 2.f;
		// Score += InstigatorSnapShotAdvanced.DamageDealt / TargetSnapshotBasic.Health;
		// Score += TargetSnapShotAdvanced.DamageDealt / InstigatorSnapshotBasic.Health;
	}
	
	bool operator==(const FCombatScore& Other) const
	{
		return Score == Other.Score;
	}

	bool operator<(const FCombatScore& Other) const
	{
		return Score < Other.Score;
	}
};

USTRUCT()
struct FGridUnitArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AGridUnitBase*> GridUnits;
	
};

USTRUCT()
struct FGridTileArray
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AGridTileBase*> GridTiles;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatData.h"
#include "UObject/Object.h"
#include "Unit/GridUnit.h"
#include "GridStructs.generated.h"

class AGridUnit;
class AGridTile;
class UWeapon;


USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FFactionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Tag;
	
	UPROPERTY()
	TMap<AGridUnit*, bool> GridUnits;

	TArray<AGridUnit*> GetGridUnits() const
	{
		TArray<AGridUnit*> OutGridUnits;
		GridUnits.GetKeys(OutGridUnits);
		return OutGridUnits;
	}
	
	TArray<AGridUnit*> GetAliveGridUnits() const
	{		
		TArray<AGridUnit*> OutGridUnits;
		for (auto Pair : GridUnits)
		{
			if (Pair.Key->GetHealth() > 0)
			{
				OutGridUnits.Add(Pair.Key);
			}
		}
		return OutGridUnits;
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

	void DeactivateUnits()
	{
		TArray<AGridUnit*> Units;
		GridUnits.GetKeys(Units);
		for (const AGridUnit* GridUnit : Units)
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

/*
 * calculate a score for each potential combat a unit can do on its turn
 */
USTRUCT()
struct FCombatScore
{
	GENERATED_BODY()

	UPROPERTY()
	AGridUnit* InstigatorUnit = nullptr;

	UPROPERTY()
	AGridTile* InstigatorTile = nullptr;

	UPROPERTY()
	FName InstigatorWeapon;
	
	UPROPERTY()
	AGridUnit* TargetUnit = nullptr;

	UPROPERTY()
	AGridTile* TargetTile = nullptr;

	UPROPERTY()
	FName TargetWeapon;
	
	UPROPERTY()
	FCombatSnapshot_Outcome CombatOutcome;
	
	UPROPERTY()
	FCombatSnapshot_Basic InstigatorSnapshotBasic;
	
	UPROPERTY()
	FCombatSnapshot_Advanced InstigatorSnapShotAdvanced;

	UPROPERTY()
	FCombatSnapshot_Basic TargetSnapshotBasic;

	UPROPERTY()
	FCombatSnapshot_Advanced TargetSnapShotAdvanced;


	// evaluate
	UPROPERTY()
	float Score = 0.f;

	void CalculateScore()
	{
		Score = 0;
		Score += InstigatorSnapShotAdvanced.HitChance / 100.f;
		Score += InstigatorSnapShotAdvanced.CriticalChance / 100.f * 2.f;
		Score += InstigatorSnapShotAdvanced.DamageDealt / TargetSnapshotBasic.Health;
		Score += TargetSnapShotAdvanced.DamageDealt / InstigatorSnapshotBasic.Health;
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
	TArray<AGridUnit*> GridUnits;
	
};

USTRUCT()
struct FGridTileArray
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AGridTile*> GridTiles;
	
};

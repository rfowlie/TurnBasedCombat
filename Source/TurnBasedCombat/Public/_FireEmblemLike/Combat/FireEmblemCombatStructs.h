// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/GridTile.h"
#include "_FireEmblemLike/Unit/FireEmblemUnit.h"


# pragma region Weapon

USTRUCT(BlueprintType)
struct FFireEmblemWeaponTraits : public FTableRowBase
{
	GENERATED_BODY()

	FFireEmblemWeaponTraits(): Might(0), Weight(0), HitRate(0), Range(0), CriticalBonus(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Might;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer SpecialTraits;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TArray<FUniqueTrait> UniqueTraits;
	
};

USTRUCT()
struct FFireEmblemWeaponContainer : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> StaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFireEmblemWeaponTraits WeaponTraits = FFireEmblemWeaponTraits();
};
# pragma endregion 

# pragma region Combat

USTRUCT(BlueprintType)
struct FFireEmblemCombatInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AFireEmblemUnit* InstigatorUnit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridTile* InstigatorTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName InstigatorWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AFireEmblemUnit* TargetUnit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridTile* TargetTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetWeapon;
};

USTRUCT(BlueprintType)
struct FFireEmblemStatsBasic
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Luck = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Resistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Movement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Constitution = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Avoid = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalRate = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalAvoid = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFireEmblemWeaponTraits WeaponTraits;
	
};

USTRUCT(BlueprintType)
struct FFireEmblemStatsAdvanced
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageDealt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackPower = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHit = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitChance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance = 0;

	// instead of determining a hit beforehand, just provide the way to calculate
	bool SimulateHit() const
	{
		return FMath::RandRange(0, 100) < HitChance;
	}
};

USTRUCT(BlueprintType)
struct FFireEmblemCombatEvaluation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float HitChanceInstigator = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float HitChanceTarget = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float CriticalChanceInstigator = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float CriticalChanceTarget = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float HealthChangePercentageInstigator = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float HealthChangePercentageTarget = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float DefeatedInstigator = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float DefeatedTarget = 1.f;

	FFireEmblemCombatEvaluation operator*(const FFireEmblemCombatEvaluation& Other) const
	{
		FFireEmblemCombatEvaluation Result;
		Result.HitChanceInstigator = HitChanceInstigator * Other.HitChanceInstigator;
		Result.HitChanceTarget = HitChanceTarget * Other.HitChanceTarget;
		Result.CriticalChanceInstigator = CriticalChanceInstigator * Other.CriticalChanceInstigator;
		Result.CriticalChanceTarget = CriticalChanceTarget * Other.CriticalChanceTarget;
		Result.HealthChangePercentageInstigator = HealthChangePercentageInstigator * Other.HealthChangePercentageInstigator;
		Result.HealthChangePercentageTarget = HealthChangePercentageTarget * Other.HealthChangePercentageTarget;
		Result.DefeatedInstigator = DefeatedInstigator * Other.DefeatedInstigator;
		Result.DefeatedTarget = DefeatedTarget * Other.DefeatedTarget;

		return Result;
	}

	float GetScore() const
	{
		return HitChanceInstigator +
			HitChanceTarget +
			CriticalChanceInstigator +
			CriticalChanceTarget +
			HealthChangePercentageInstigator +
			HealthChangePercentageTarget +
			DefeatedInstigator +
			DefeatedTarget;
	}
};

USTRUCT(BlueprintType)
struct FFireEmblemCombatPrediction
{
	GENERATED_BODY()

	// testing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FGuid Id;

	// Constructor that generates a new GUID when the struct is created
	FFireEmblemCombatPrediction()
	{
		Id = FGuid::NewGuid();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFireEmblemCombatInformation CombatInformation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFireEmblemStatsBasic InstigatorSnapshotBasic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFireEmblemStatsAdvanced InstigatorSnapShotAdvanced;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFireEmblemStatsBasic TargetSnapshotBasic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFireEmblemStatsAdvanced TargetSnapshotAdvanced;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AFireEmblemUnit*> CombatOrder = TArray<AFireEmblemUnit*>();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFireEmblemCombatEvaluation CombatEvaluation;

	// so that we can sort arrays of predictions
	bool operator==(const FFireEmblemCombatPrediction& Other) const
	{
		return CombatEvaluation.GetScore() == Other.CombatEvaluation.GetScore();
	}
	
	bool operator<(const FFireEmblemCombatPrediction& Other) const
	{
		return CombatEvaluation.GetScore() < Other.CombatEvaluation.GetScore();
	}

	// debug
	FString GetString() const
	{
		FString Output;
		// Output.Append(Id.ToString());
		if (IsValid(CombatInformation.InstigatorUnit))
		{
			Output.Append(CombatInformation.InstigatorUnit->GetActorNameOrLabel());
		}
		Output.Append(" - ");
		if (IsValid(CombatInformation.InstigatorUnit))
		{
			Output.Append(CombatInformation.TargetUnit->GetActorNameOrLabel());
		}
		Output.Append(" - ");
		Output.Append(Id.ToString());

		return Output;
	}
};

USTRUCT(BlueprintType)
struct FFireEmblemCombatOutcome
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AFireEmblemUnit* Instigator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFireEmblemStatsAdvanced InstigatorSnapshot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InstigatorAttacks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AFireEmblemUnit* Target = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFireEmblemStatsAdvanced TargetSnapshot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetAttacks = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AFireEmblemUnit*> CombatOrder = TArray<AFireEmblemUnit*>();
};

/*
 * calculate a score for each potential combat a unit can do on its turn
 */
USTRUCT()
struct FFireEmblemCombatScore
{
	GENERATED_BODY()

	UPROPERTY()
	AFireEmblemUnit* InstigatorUnit = nullptr;

	UPROPERTY()
	AGridTile* InstigatorTile = nullptr;

	UPROPERTY()
	FName InstigatorWeapon;
	
	UPROPERTY()
	AFireEmblemUnit* TargetUnit = nullptr;

	UPROPERTY()
	AGridTile* TargetTile = nullptr;

	UPROPERTY()
	FName TargetWeapon;
	
	UPROPERTY()
	FFireEmblemCombatOutcome CombatOutcome;
	
	UPROPERTY()
	FFireEmblemStatsBasic InstigatorSnapshotBasic;
	
	UPROPERTY()
	FFireEmblemStatsAdvanced InstigatorSnapShotAdvanced;

	UPROPERTY()
	FFireEmblemStatsBasic TargetSnapshotBasic;

	UPROPERTY()
	FFireEmblemStatsAdvanced TargetSnapShotAdvanced;


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
	
	bool operator==(const FFireEmblemCombatScore& Other) const
	{
		return Score == Other.Score;
	}

	bool operator<(const FFireEmblemCombatScore& Other) const
	{
		return Score < Other.Score;
	}
};
# pragma endregion 
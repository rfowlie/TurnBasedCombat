// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsDataAsset.generated.h"

// final snapshot of what a units stats are after getting results from ranges
USTRUCT(BlueprintType)
struct FUnitStatsSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Mana = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Dexterity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magic = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Resistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Luck = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Movement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MovementRecovery = 0;

	FUnitStatsSnapshot operator+(const FUnitStatsSnapshot& Other) const
	{
		FUnitStatsSnapshot Temp;
		Temp.Health = Health + Other.Health;
		Temp.Mana = Mana + Other.Mana;
		Temp.Strength = Strength + Other.Strength;
		Temp.Defence = Defence + Other.Defence;
		Temp.Speed = Speed + Other.Speed;
		Temp.Dexterity = Dexterity + Other.Dexterity;
		Temp.Magic = Magic + Other.Magic;
		Temp.Resistance = Resistance + Other.Resistance;
		Temp.Luck = Luck + Other.Luck;

		// TODO: finalize and finish...


		return Temp;
	};
	
	FUnitStatsSnapshot& operator+=(const FUnitStatsSnapshot& Other)
	{
		Health += Other.Health;
		Mana += Other.Mana;
		Strength += Other.Strength;
		Defence += Other.Defence;
		Speed += Other.Speed;
		Dexterity += Other.Dexterity;
		Magic += Other.Magic;
		Resistance += Other.Resistance;
		Luck += Other.Luck;

		// TODO: finalize and finish...

		return *this;
	};
};

// container for stat ranging
USTRUCT(BlueprintType)
struct FStatRange
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 StatMin = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 StatMax = 99;
	
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* GrowthCurve = nullptr;
	
};

/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASEDCOMBAT_API UStatsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FStatRange Health;
	
	UPROPERTY(EditDefaultsOnly)
	FStatRange Mana;
	
	UPROPERTY(EditDefaultsOnly)
	FStatRange Strength;

	UPROPERTY(EditDefaultsOnly)
	FStatRange Defence;

	UPROPERTY(EditDefaultsOnly)
	FStatRange Skill;
	
	UPROPERTY(EditDefaultsOnly)
	FStatRange Speed;

	UPROPERTY(EditDefaultsOnly)
	FStatRange Magic;

	UPROPERTY(EditDefaultsOnly)
	FStatRange Resistance;

	UPROPERTY(EditDefaultsOnly)
	FStatRange Luck;

	UPROPERTY(EditDefaultsOnly)
	FStatRange Movement;

	UPROPERTY(EditDefaultsOnly)
	FStatRange MovementRecovery;
	
	UPROPERTY(EditDefaultsOnly)
	FStatRange Constitution;
	
	UFUNCTION(BlueprintCallable)
	void GetStats(FUnitStatsSnapshot& UnitStats, const int32 Level) const;

private:
	float GetStatValue(const FStatRange& StatRange, const float Level) const;
};

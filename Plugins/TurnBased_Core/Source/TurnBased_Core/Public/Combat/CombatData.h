// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "Tile/GridTile.h"
#include "Weapon/WeaponData.h"
#include "CombatData.generated.h"


class AGridUnit;
class UGridManager;


// Fire Emblem ~ Base Stats
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Strength);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Skill);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Speed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Luck);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Defence);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Resistance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Movement);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Constitution);

// Fire Emblem ~ Calculated Stats
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_AttackSpeed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_HitRate);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Avoid);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Accuracy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_AttackPower);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_DefencePower);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_CriticalDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_CriticalRate);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_CriticalEvade);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Stat_CriticalChance);


USTRUCT(BlueprintType)
struct FCombatSnapshot_Basic
{
	GENERATED_BODY()

	FCombatSnapshot_Basic() {}

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
	FWeaponTraits WeaponTraits;
	
};

USTRUCT(BlueprintType)
struct FCombatSnapshot_Advanced
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
struct FCombatSnapshot_Outcome
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridUnit* Instigator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatSnapshot_Advanced InstigatorSnapshot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InstigatorAttacks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridUnit* Target = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatSnapshot_Advanced TargetSnapshot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetAttacks = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AGridUnit*> CombatOrder = TArray<AGridUnit*>();
};


// hold all the relevant information about the combatants
// any other potentially required information can be added here or found out using subsystems etc.
USTRUCT(BlueprintType)
struct FCombatInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridUnit* InstigatorUnit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridTile* InstigatorTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName InstigatorWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridUnit* TargetUnit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridTile* TargetTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetWeapon;
};


USTRUCT(BlueprintType)
struct FCombatEvaluation
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

	FCombatEvaluation operator*(const FCombatEvaluation& Other) const
	{
		FCombatEvaluation Result;
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
struct FCombatPrediction
{
	GENERATED_BODY()

	// testing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FGuid Id;

	// Constructor that generates a new GUID when the struct is created
	FCombatPrediction()
	{
		Id = FGuid::NewGuid();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatInformation CombatInformation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatSnapshot_Basic InstigatorSnapshotBasic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatSnapshot_Advanced InstigatorSnapShotAdvanced;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatSnapshot_Basic TargetSnapshotBasic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatSnapshot_Advanced TargetSnapshotAdvanced;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AGridUnit*> CombatOrder = TArray<AGridUnit*>();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatEvaluation CombatEvaluation;

	// so that we can sort arrays of predictions
	bool operator==(const FCombatPrediction& Other) const
	{
		return CombatEvaluation.GetScore() == Other.CombatEvaluation.GetScore();
	}
	
	bool operator<(const FCombatPrediction& Other) const
	{
		return CombatEvaluation.GetScore() < Other.CombatEvaluation.GetScore();
	}
};

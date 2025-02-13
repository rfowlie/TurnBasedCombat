// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
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
	int32 HealthChange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackPower = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHit = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitChance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance = 0;
};

USTRUCT(BlueprintType)
struct FCombatSnapshot_Outcome
{
	GENERATED_BODY()
	
	FCombatSnapshot_Outcome(): InstigatorAttacks(0), TargetAttacks(0)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridUnit* Instigator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatSnapshot_Advanced InstigatorSnapshot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InstigatorAttacks;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridUnit* Target = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatSnapshot_Advanced TargetSnapshot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetAttacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AGridUnit*> CombatOrder = TArray<AGridUnit*>();
};

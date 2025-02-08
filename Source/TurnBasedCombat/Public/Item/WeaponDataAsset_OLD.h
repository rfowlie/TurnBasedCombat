// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset_OLD.generated.h"


UENUM()
enum class EWeaponDamageType
{
	Physical,
	Magic,
	Pure,
	Ethereal,
	Toxic
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 Encumbrance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 Durability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponDamageType WeaponDamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1, ClampMax = 99, UIMin = 1, UIMax = 99))
	int32 Effort;	
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UWeaponDataAsset_OLD : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponStats WeaponStats;
};

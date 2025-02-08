// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Might);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Weight);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Hit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Range);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Special_Light);


// USTRUCT(BlueprintType)
// struct FWeaponSolverStruct : public FTableRowBase
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TSubclassOf<UStatSolver> StatSolver = nullptr;
// };

// USTRUCT(BlueprintType)
// struct FUniqueTrait
// {
// 	GENERATED_BODY()
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FGameplayTag Trait;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TSubclassOf<UStatSolver> StatSolver = nullptr;
// };


USTRUCT(BlueprintType)
struct FWeaponTraits : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponTraits(): Might(0), Weight(0), HitRate(0), Range(0), CriticalBonus(0)
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
struct FWeaponContainer : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> StaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponTraits WeaponTraits = FWeaponTraits();
};

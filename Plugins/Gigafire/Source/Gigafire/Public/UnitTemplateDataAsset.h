// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GigafireStructs.h"
#include "UnrealGameStructs.h"
#include "UnitTemplateDataAsset.generated.h"


/*
 * FOR NOW
 * this is low level and will be dependent on the games overall design
 */
USTRUCT(BlueprintType)
struct FItemSlotDescriptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Gigafire.Slot.Type"))
	FGameplayTagContainer Types;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Clampmin = "1"))
	signed Level = 1;
	
};

USTRUCT(BlueprintType)
struct FItemSlotEquippedData
{
	GENERATED_BODY()

	// what unit/class/etc. that owns this slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Gigafire.Unit"))
	FGameplayTag TemplateTag;

	// what index of the owning unit/class/etc. so that we can check validity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Clampmin = "0"))
	int32 Index = 0;

	// FGuid allows us to be generic and to associate various item setups to the same base system
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid ItemGuid;
};



/*
 * Unit load data will convert to this for saving as TMaps do not serialize well
 */
USTRUCT(BlueprintType)
struct FUnitSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag UnitTag;

	// used to find UUnitTemplateDataAssets that unit owns
	// allows generic usage as there could be one or many templates a unit is derived from
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTagValuePair> Templates;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemSlotEquippedData> EquippedItems;
	
	// permanent or unique qualities for units based on gameplay or decisions
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTagValuePair> PermanentStats;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTagValuePair> PermanentTraits;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTagValuePair> PermanentSkills;
	
};

/*
 * Unit save data will convert to this after load for less nested structs and ease of use
 * Instead of storing unit/class/etc. simply store the relevant tag and level then find and use accordingly
 */
USTRUCT(BlueprintType)
struct FUnitLoadData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Gigafire.Unit.Character"))
	FGameplayTag UnitTag;
	
	// used to find UUnitTemplateDataAssets that unit has
	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Gigafire.Unit.Class"))
	TMap<FGameplayTag, float> Templates;

	// store what items are stored in which slots for which unit template
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemSlotEquippedData> EquippedData;
	
	// permanent adjustments
	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Gigafire.Stat"))
	TMap<FGameplayTag, float> PermanentStats;
	
	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Gigafire.Trait"))
	TMap<FGameplayTag, float> PermanentTraits;
	
	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Gigafire.Skill"))
	TMap<FGameplayTag, float> PermanentSkills;
	
};

/**
 * GAS friendly outliner for defining unit/class default values
 * use tag value maps to enable generic use
 */
UCLASS(BlueprintType, Blueprintable)
class GIGAFIRE_API UUnitTemplateDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gigafire.Unit"))
	FGameplayTag TemplateTag;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gigafire.Stat"))
	TMap<FGameplayTag, float> Stats;

	// passive effects like boons or curses
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gigafire.Trait"))
	TMap<FGameplayTag, float> Traits;
	
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gigafire.Skill"))
	TMap<FGameplayTag, float> Skills;

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemSlotDescriptionData> ItemSlotDescriptions;
};


/*
 * used to allow data tables to be created for unit stat definitions as an alternative to data asset
 * mainly we can easily associate a gameplay tag with a setup, although this can be error-prone if tags change
 */
USTRUCT(BlueprintType)
struct FUnitStatsTemplate : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Gigafire.Stat"))
	TMap<FGameplayTag, float> Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Gigafire.Trait"))
	TMap<FGameplayTag, float> Traits;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Gigafire.Skill"))
	TMap<FGameplayTag, float> Skills;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemSlotDescriptionData> ItemSlotDescriptions;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "EmblemSystemStructs.generated.h"

class UEmblemItem;


USTRUCT(BlueprintType)
struct FEmblemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMergeLevelAllowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEmblemItem* EquippedEmblem = nullptr;
};

#pragma region Item

USTRUCT(BlueprintType)
struct FEmblemStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StatTag; // e.g. "Strength", "Defense", "Speed"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value; // e.g. +3
};

USTRUCT(BlueprintType)
struct FEmblemAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AbilityTag;

	// TODO: merge cost will be in some look up table
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int32 MergeCost; // how much of the merge cap this ability consumes
};

#pragma endregion

#pragma region Save

USTRUCT(BlueprintType)
struct FEmblemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGuid UniqueId;

	UPROPERTY(BlueprintReadWrite)
	int32 MergeCap = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MergeLevel = 0;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer EmblemType;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEmblemStat> Stats;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEmblemAbility> Abilities;
};

USTRUCT(BlueprintType)
struct FEmblemSlotData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = -1;

	// types of emblems this slot supports
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer EmblemType;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MergeLevel = 0;

	UPROPERTY(BlueprintReadWrite)
	FGuid EmblemDataUniqueId; // Empty if no emblem equipped
};


/*
 * instead of storing all the character data just keep track of character emblem data
 */
USTRUCT(BlueprintType)
struct FCharacterEmblemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag CharacterTag;

	/*
	 * class tag will determine the number and types of emblem slots for the character
	 * at some point, probably at load we will need to check that this and the equipped slots
	 * align, if not then reset the equipped slots
	 * this will prevent hacks
	*/
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ClassTag;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FEmblemSlotData> EquippedSlots;
};

#pragma endregion
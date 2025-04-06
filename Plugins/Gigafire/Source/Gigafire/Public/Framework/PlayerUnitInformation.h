// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Elements/Common/TypedElementHandles.h"
#include "UObject/Object.h"
#include "PlayerUnitInformation.generated.h"


// for data table
USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> EffectedTagsAndValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> StaticMesh;
};

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Type;

	// this will store the key for the ItemTableRow Database
	UPROPERTY()
	FName Key;
};


/**
 * 
 */
UCLASS(BlueprintType)
class GIGAFIRE_API UPlayerUnitInformation : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Name;

	// base stats

	// permanent items

	UPROPERTY()
	TArray<FItemSlot> Slots;
};

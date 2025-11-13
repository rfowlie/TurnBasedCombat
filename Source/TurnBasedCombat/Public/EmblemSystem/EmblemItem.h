// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EmblemSystemStructs.h"
#include "EmblemItem.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UEmblemItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FGuid UniqueId; // Unique per character (could be player guid, or pawn id)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MergeCap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MergeLevel; // Current merge level (0..Cap)

	// Max merges allowed (e.g. 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEmblemStat> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEmblemAbility> Abilities;
};

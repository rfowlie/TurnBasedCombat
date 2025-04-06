#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EmblemStructs.generated.h"


USTRUCT(BlueprintType)
struct FEmblem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> EmblemTags;
	
};

USTRUCT(BlueprintType)
struct FEmblemSlot
{
	GENERATED_BODY()

	FEmblemSlot() {}
	explicit FEmblemSlot(const FGameplayTag EmblemSlotTag) : EmblemSlotType(EmblemSlotTag) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EmblemSlotType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEmblem Emblem;
	
};

USTRUCT(BlueprintType)
struct FEmblemSlotContainer
{
	GENERATED_BODY()

	FEmblemSlotContainer() {}
	explicit FEmblemSlotContainer(const TArray<FGameplayTag>& EmblemSlotTypes)
	{
		for (const FGameplayTag& SlotType : EmblemSlotTypes)
		{
			EmblemSlots.Add(FEmblemSlot(SlotType));
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEmblemSlot> EmblemSlots;
};
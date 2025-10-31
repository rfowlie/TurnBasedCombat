// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "GigafireStructs.h"
#include "GigafireSaveGame.generated.h"


struct FGameplayTag;
struct FUnitLoadData;
struct FUnitSaveData;


USTRUCT(BlueprintType)
struct FItemEmblem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGuid UniqueId;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer ItemTypes;

	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTagValuePair> Stats;

	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayTagValuePair> Abilities;

	UPROPERTY(BlueprintReadWrite)
	int32 MergeCap = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MergeLevel = 0;

	bool operator==(const FItemEmblem& ToCompare) const
	{
		return UniqueId == ToCompare.UniqueId;
	};	
};

/**
 * 
 */
UCLASS()
class GIGAFIRE_API UGigafireSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// units
	UPROPERTY(BlueprintReadWrite)
	TArray<FUnitSaveData> UnitSaveData;

	// items
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemEmblem> EmblemSaveData;

};


/**
 * How we want other objects to be able to interact with the save game data (model)
 */
UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API UGigafireSaveGameViewModel : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGigafireSaveGame* SaveGame = nullptr;

	bool IsSaveGameNull() const
	{
		if (!SaveGame)
		{
			UE_LOG(LogTemp, Error, TEXT("GigafireSaveGameViewModel::CheckSaveGameValid - SaveGame is Null"));
			return true;
		}
		
		return false;
	}
	
public:
	UFUNCTION(BlueprintCallable, Category="ViewModel")
	void SetSaveGame(UGigafireSaveGame* InSaveGame);
	
	// stats
	UFUNCTION(BlueprintCallable)
	bool GetUnitData(FUnitLoadData& OutLoadData, FGameplayTag UnitTag) const;

	UFUNCTION(BlueprintCallable, Category="ViewModel|Unit")
	TArray<FUnitLoadData> GetAllUnitData() const;

	UFUNCTION(BlueprintCallable, Category="ViewModel|Unit")
	TArray<FGameplayTag> GetAllUnitTags() const;
	
	// emblem
	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	bool AddItemEmblem(const FItemEmblem& InItemEmblem) const;
	
	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	bool RemoveItemEmblem(const FGuid& InGuid) const;

	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	bool MergeItemEmblems(const FGuid& InGuid);
	
	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem|Filter")
	bool GetItemEmblemByGuid(FItemEmblem& OutItemEmblem, FGuid EmblemId) const;
	
	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem|Filter")
	TArray<FItemEmblem> GetItemEmblemsByTypeAndLevel(FGameplayTagContainer Types, int32 Level) const;

	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem|Filter")
	bool GetEmblemInItemSlot(FItemEmblem& OutItemEmblem, const FGameplayTag UnitTag, const FGameplayTag TemplateTag, const signed Index);

	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	bool SetEmblemInItemSlot(const FItemEmblem& InItemEmblem, const FGameplayTag UnitTag, const FGameplayTag TemplateTag, const int32 Index);

	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	void RemoveEmblemInItemSlot(const FItemEmblem& InItemEmblem);
	
};
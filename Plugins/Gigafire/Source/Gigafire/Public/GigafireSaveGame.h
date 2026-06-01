// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "GigafireStructs.h"
#include "UnrealGameStructs.h"
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

UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API UGigafireBaseViewModel : public UObject
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
	
	UFUNCTION(BlueprintCallable, Category="ViewModel|Unit")
	TArray<FGameplayTag> GetAllUnitTags() const;
	
};

/**
 * How we want other objects to be able to interact with the save game data (model)
 */
UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API UGigafireSaveGameViewModel : public UGigafireBaseViewModel
{
	GENERATED_BODY()

public:
	// stats
	UFUNCTION(BlueprintCallable)
	bool GetUnitData(FUnitLoadData& OutLoadData, FGameplayTag UnitTag) const;

	UFUNCTION(BlueprintCallable, Category="ViewModel|Unit")
	TArray<FUnitLoadData> GetAllUnitData() const;
	
	// emblem
	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	bool AddItemEmblemToInventory(const FItemEmblem& InItemEmblem) const;
	
	UFUNCTION(BlueprintCallable, Category="ViewModel|Emblem")
	bool RemoveItemEmblemFromInventory(const FGuid& InGuid) const;

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


USTRUCT(BlueprintType)
struct FGigafireUnitAttributes
{
	GENERATED_BODY()
	
	/*
	 * regeneration will be a fundamental aspect of gameplay
	 * we do not want one to one stats for regeneration, each primary stats regeneration should
	 * be based on a combination of stats or gameplay factors
	 */ 

	// regeneration
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealthRegeneration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ManaRegeneration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnergyRegeneration = 0;
	
	// primary
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Mana = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Energy = 0;

	// secondary
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magic = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Resistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Luck = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Movement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MovementRecovery = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Constitution = 0;
	
};

UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API UGigafireCombatViewModel : public UGigafireBaseViewModel
{
	GENERATED_BODY()

public:
	/*
	 * make it blueprint implementable because we want designers to be able to determine
	 * the conversion of tags to attributes
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="ViewModel|Combat")
	void GetUnitAttributesByTag(FGigafireUnitAttributes& OutAttributes, const FGameplayTag UnitTag) const;
	
};
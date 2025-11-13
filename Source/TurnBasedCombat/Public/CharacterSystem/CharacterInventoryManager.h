// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EmblemSystem/EmblemSystemStructs.h"
#include "CharacterInventoryManager.generated.h"


USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FCharacterData
{
	GENERATED_BODY()

public:
	/** Unique identifier for the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CharacterTag;

	/** Unit class reference (Data Asset holding base stats, abilities, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UDataAsset> UnitClass;

	/** Array of emblem slot data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEmblemSlotData> EmblemSlots;
	
};

/**
 * Manager for handling unique characters and their equipped emblems
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API UCharacterInventoryManager : public UObject
{
	GENERATED_BODY()

public:
	/** Load all characters from a SaveGame slot */
	UFUNCTION(BlueprintCallable, Category="Inventory|Characters")
	bool LoadFromSlot(int32 UserIndex);
	
	/** Adds or updates a character */
	UFUNCTION(BlueprintCallable, Category="Inventory|Characters")
	void AddOrUpdateCharacter(const FCharacterData& CharacterData);

	/** Removes a character */
	UFUNCTION(BlueprintCallable, Category="Inventory|Characters")
	void RemoveCharacter(FGameplayTag CharacterTag);

	/** Get character data */
	UFUNCTION(BlueprintCallable, Category="Inventory|Characters")
	bool GetCharacter(FCharacterData& OutData, FGameplayTag CharacterTag) const;

	/** Get all characters */
	UFUNCTION(BlueprintCallable, Category="Inventory|Characters")
	const TArray<FCharacterData>& GetAllCharacters() const { return Characters; }

private:
	/** All characters */
	UPROPERTY()
	TArray<FCharacterData> Characters;
};

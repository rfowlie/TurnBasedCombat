// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmblemSystemStructs.h"
#include "EmblemInventoryManager.generated.h"

class UEmblemSaveGame;


/**
 * UObject-based inventory container for storing and managing emblems.
 * Lives inside the GameInstance.
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API UEmblemInventoryManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool LoadInventory(const int UserIndex);

	UFUNCTION()
	bool SaveInventory(const int UserIndex);
	
	/** All emblems owned by the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FEmblemData> Inventory;

	/** Equips an emblem for a character in a given slot */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void EquipEmblem(FName CharacterID, int32 SlotIndex, const FEmblemData& Emblem);

	/** Unequips emblem from a given slot */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UnequipEmblem(FName CharacterID, int32 SlotIndex);

	/** Gets currently equipped emblems for a character */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	const TArray<FEmblemData>& GetEquippedEmblems(FName CharacterID) const;

private:
	const FString SlotName = "EmblemSlot";
	
	UPROPERTY()
	UEmblemSaveGame* SaveGame = nullptr;
	
};

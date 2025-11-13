// Fill out your copyright notice in the Description page of Project Settings.


#include "EmblemSystem/EmblemInventoryManager.h"

#include "EmblemSystem/EmblemSaveGame.h"
#include "Kismet/GameplayStatics.h"


bool UEmblemInventoryManager::LoadInventory(const int UserIndex)
{
	SaveGame = Cast<UEmblemSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	
	if (SaveGame == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load from slot '%s' index %d"), *SlotName, UserIndex);
		return false;
	}
    
	return true;
}

bool UEmblemInventoryManager::SaveInventory(const int UserIndex)
{
	UEmblemSaveGame* SaveObj = Cast<UEmblemSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UEmblemSaveGame::StaticClass()));
	if (SaveObj == nullptr) { return false; }
	
	// TODO: populate save

	return UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex);
}

void UEmblemInventoryManager::EquipEmblem(FName CharacterID, int32 SlotIndex, const FEmblemData& Emblem)
{
	// if (!EquippedEmblems.Contains(CharacterID))
	// {
	// 	EquippedEmblems.Add(CharacterID, TArray<FEmblemData>());
	// }
	//
	// TArray<FEmblemData>& Slots = EquippedEmblems[CharacterID];
	//
	// if (Slots.Num() <= SlotIndex)
	// {
	// 	Slots.SetNum(SlotIndex + 1); // resize to fit slot
	// }
	//
	// Slots[SlotIndex] = Emblem;
}

void UEmblemInventoryManager::UnequipEmblem(FName CharacterID, int32 SlotIndex)
{
	// if (EquippedEmblems.Contains(CharacterID))
	// {
	// 	TArray<FEmblemData>& Slots = EquippedEmblems[CharacterID];
	// 	if (Slots.IsValidIndex(SlotIndex))
	// 	{
	// 		Slots[SlotIndex] = FEmblemData(); // reset to default
	// 	}
	// }
}

const TArray<FEmblemData>& UEmblemInventoryManager::GetEquippedEmblems(FName CharacterID) const
{
	static TArray<FEmblemData> EmptyArray;
	// if (const TArray<FEmblemData>* Found = EquippedEmblems.Find(CharacterID))
	// {
	// 	return *Found;
	// }
	return EmptyArray;
}

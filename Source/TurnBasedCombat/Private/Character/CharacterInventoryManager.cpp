// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterInventoryManager.h"

#include "Character/CharacterSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

bool UCharacterInventoryManager::LoadFromSlot(int32 UserIndex)
{
	  // Use a fixed slot name
        const FString SlotName = "CharacterSlot";
    
        if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex))
        {
            if (UCharacterSaveGame* CharSave = Cast<UCharacterSaveGame>(SaveGame))
            {
                // Clear current characters and load from save
                Characters.Empty();
                Characters.Append(CharSave->SavedCharacters);
                return true;
            }
        }
    
        // Could not load
        UE_LOG(LogTemp, Warning, TEXT("Failed to load from slot '%s' index %d"), *SlotName, UserIndex);
        return false;
}

void UCharacterInventoryManager::AddOrUpdateCharacter(const FCharacterData& CharacterData)
{
	for (FCharacterData& Existing : Characters)
	{
		if (Existing.CharacterTag == CharacterData.CharacterTag)
		{
			Existing = CharacterData; // update
			return;
		}
	}

	Characters.Add(CharacterData); // add new
}

void UCharacterInventoryManager::RemoveCharacter(FGameplayTag CharacterTag)
{
	Characters.RemoveAll([&](const FCharacterData& Data)
	{
		return Data.CharacterTag == CharacterTag;
	});
}

bool UCharacterInventoryManager::GetCharacter(FCharacterData& OutData, FGameplayTag CharacterTag) const
{
	for (const FCharacterData& Existing : Characters)
	{
		if (Existing.CharacterTag == CharacterTag)
		{
			OutData = Existing;
			return true;
		}
	}
	return false;
}


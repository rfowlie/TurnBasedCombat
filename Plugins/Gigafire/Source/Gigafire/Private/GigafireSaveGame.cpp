// Fill out your copyright notice in the Description page of Project Settings.


#include "GigafireSaveGame.h"
#include "UnitTemplateDataAsset.h"
#include "UnitTemplateUtility.h"


void UGigafireBaseViewModel::SetSaveGame(UGigafireSaveGame* InSaveGame)
{
	if (!InSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("UGigafireSaveGameViewModel::SetSaveGame - InSaveGame is Null"))
	}
	if (SaveGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGigafireSaveGameViewModel::SetSaveGame - SaveGame already exists, replacing..."))
	}

	SaveGame = InSaveGame;
}

TArray<FGameplayTag> UGigafireBaseViewModel::GetAllUnitTags() const
{
	TArray<FGameplayTag> Output;
	if (IsSaveGameNull()) return Output;
	for (auto SaveData : SaveGame->UnitSaveData)
	{
		Output.Add(SaveData.UnitTag);
	}
	return Output;
}

bool UGigafireSaveGameViewModel::GetUnitData(FUnitLoadData& OutLoadData, const FGameplayTag UnitTag) const
{
	if (IsSaveGameNull()) return false;
	FUnitLoadData OutputData;
	for (auto SaveData : SaveGame->UnitSaveData)
	{
		if (SaveData.UnitTag == UnitTag)
		{
			UUnitTemplateUtility::MakeUnitLoadData(SaveData, OutLoadData);
			return true;
		}
	}

	return false;
}

TArray<FUnitLoadData> UGigafireSaveGameViewModel::GetAllUnitData() const
{
	TArray<FUnitLoadData> Output;
	if (IsSaveGameNull()) return Output;
	for (auto SaveData : SaveGame->UnitSaveData)
	{
		FUnitLoadData OutLoadData;
		UUnitTemplateUtility::MakeUnitLoadData(SaveData, OutLoadData);
		Output.Add(OutLoadData);
	}

	return Output;
}

bool UGigafireSaveGameViewModel::AddItemEmblemToInventory(const FItemEmblem& InItemEmblem) const
{
	if (IsSaveGameNull()) return false;
	// check if Guid already exists, do not want to allow emblems being edited
	for (const auto Emblem : SaveGame->EmblemSaveData)
	{
		if (Emblem.UniqueId == InItemEmblem.UniqueId)
		{
			UE_LOG(LogTemp, Warning, TEXT("GigafireSaveGame::AddItemEmblem - Emblem with Guid already exists"))
			return false;
		}		
	}

	SaveGame->EmblemSaveData.Add(InItemEmblem);
	return true;
}

bool UGigafireSaveGameViewModel::RemoveItemEmblemFromInventory(const FGuid& InGuid) const
{
	if (IsSaveGameNull()) return false;
	for (signed Index = 0; Index < SaveGame->EmblemSaveData.Num(); Index++)
	{
		if (SaveGame->EmblemSaveData[Index].UniqueId == InGuid)
		{
			SaveGame->EmblemSaveData.RemoveAt(Index);
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT(
			"GigafireSaveGame::RemoveItemEmblem - No Emblem with Guid %s exists"), *InGuid.ToString());
	return false;
}

bool UGigafireSaveGameViewModel::MergeItemEmblems(const FGuid& InGuid)
{
	// TODO: how do we want this to work
	UE_LOG(LogTemp, Error, TEXT("UGigafireSaveGameViewModel::MergeItemEmblems - TODO"))
	return false;

}

bool UGigafireSaveGameViewModel::GetItemEmblemByGuid(FItemEmblem& OutItemEmblem, const FGuid EmblemId) const
{
	if (!IsSaveGameNull())
	{
		for (auto ItemEmblem : SaveGame->EmblemSaveData)
		{
			if (EmblemId == ItemEmblem.UniqueId)
			{
				OutItemEmblem = ItemEmblem;
				return true;
			}
		}
	}	

	return false;
}

// TODO: this is going to require some thinking to determine all possibilities
// for now just match exact
TArray<FItemEmblem> UGigafireSaveGameViewModel::GetItemEmblemsByTypeAndLevel(const FGameplayTagContainer Types, const int32 Level) const
{
	UE_LOG(LogTemp, Error, TEXT("UGigafireSaveGameViewModel::GetItemEmblemsByTypeAndLevel - TODO"))

	TArray<FItemEmblem> Output;
	if (!IsSaveGameNull())
	{
		for (auto ItemEmblem : SaveGame->EmblemSaveData)
		{
			if (ItemEmblem.ItemTypes.HasAnyExact(Types) && ItemEmblem.MergeLevel <= Level)
			{
				Output.Add(ItemEmblem);
			}
		}
	}	

	return Output;
}

bool UGigafireSaveGameViewModel::GetEmblemInItemSlot(
	FItemEmblem& OutItemEmblem, const FGameplayTag UnitTag, const FGameplayTag TemplateTag, const signed int Index)
{
	if (IsSaveGameNull()){ return false; }

	for (const FUnitSaveData& UnitSaveData : SaveGame->UnitSaveData)
	{
		if (UnitSaveData.UnitTag != UnitTag) { continue; }

		const FItemSlotEquippedData* FoundItem = UnitSaveData.EquippedItems.FindByPredicate(
			[&](const FItemSlotEquippedData& Item)
			{
				return Item.TemplateTag == TemplateTag && Item.Index == Index;
			});

		if (FoundItem)
		{
			GetItemEmblemByGuid(OutItemEmblem, FoundItem->ItemGuid);
			return true;
		}

		// If the unit exists but does not have a matching slot
		UE_LOG(LogTemp, Verbose, TEXT(
			"UGigafireSaveGameViewModel::GetItemEmblemInSlot - No equipped item found for %s - %s - %d"),
			*UnitTag.ToString(), *TemplateTag.ToString(), Index);

		return false;
	}

	return false;
}

// TODO: check all other units and remove slots that have matching emblem
bool UGigafireSaveGameViewModel::SetEmblemInItemSlot(
	const FItemEmblem& InItemEmblem, const FGameplayTag UnitTag, const FGameplayTag TemplateTag, const int32 Index)
{
	if (IsSaveGameNull()){ return false; }

	for (FUnitSaveData& UnitSaveData : SaveGame->UnitSaveData)
	{
		if (UnitSaveData.UnitTag != UnitTag) { 	continue; }

		const int32 MatchingIndex = UnitSaveData.EquippedItems.IndexOfByPredicate(
			[&](const FItemSlotEquippedData& Item)
			{
				return Item.TemplateTag == TemplateTag && Item.Index == Index;
			}
		);

		if (MatchingIndex != INDEX_NONE)
		{
			// Update existing equipped item
			UnitSaveData.EquippedItems[MatchingIndex].ItemGuid = InItemEmblem.UniqueId;
			UE_LOG(LogTemp, Log, TEXT("Match Found Updating: %d"), UnitSaveData.EquippedItems.Num());
		}
		else
		{
			// Add new equipped item
			FItemSlotEquippedData NewItem;
			NewItem.TemplateTag = TemplateTag;
			NewItem.Index = Index;
			NewItem.ItemGuid = InItemEmblem.UniqueId;
			UE_LOG(LogTemp, Log, TEXT("Before Add: %d"), UnitSaveData.EquippedItems.Num());
			UnitSaveData.EquippedItems.Add(NewItem);
			UE_LOG(LogTemp, Log, TEXT("After Add: %d"), UnitSaveData.EquippedItems.Num());
		}

		return true;
	}

	return false;
}

void UGigafireSaveGameViewModel::RemoveEmblemInItemSlot(const FItemEmblem& InItemEmblem)
{
	if (IsSaveGameNull()){ return; }

	for (FUnitSaveData& UnitSaveData : SaveGame->UnitSaveData)
	{
		const int32 MatchingIndex = UnitSaveData.EquippedItems.IndexOfByPredicate(
			[&](const FItemSlotEquippedData& Item)
			{
				return Item.ItemGuid == InItemEmblem.UniqueId;
			}
		);

		if (MatchingIndex != INDEX_NONE)
		{
			UnitSaveData.EquippedItems.RemoveAt(MatchingIndex);
		}
	}
}


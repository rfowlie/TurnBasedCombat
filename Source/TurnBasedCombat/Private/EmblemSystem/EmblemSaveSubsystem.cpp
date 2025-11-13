// Fill out your copyright notice in the Description page of Project Settings.

#include "EmblemSaveSubsystem.h"

#include "IPlatformFilePak.h"
#include "EmblemSystem/EmblemSaveGame.h"
#include "Kismet/GameplayStatics.h"


void UEmblemSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// TODO: where are we pulling the user index from?
	LoadOrCreateSave(0);
}

void UEmblemSaveSubsystem::NotifyDataChanged() const
{
	if (OnSaveDataChanged.IsBound()) { OnSaveDataChanged.Broadcast(); }
}

FEmblemData UEmblemSaveSubsystem::GetEmblemDataById(const FGuid& EmblemId) const
{
	if (!CachedSave) { return FEmblemData(); }
	
	const FEmblemData* FoundData = CachedSave->SavedInventory.FindByPredicate(
		[&](const FEmblemData& Data)
		{
			return Data.UniqueId == EmblemId;
		});
	
	return FoundData ? *FoundData : FEmblemData();
}

FCharacterEmblemData UEmblemSaveSubsystem::GetCharacterDataByTag(const FGameplayTag& CharacterTag) const
{
	if (!CachedSave) { return FCharacterEmblemData(); }

	const FCharacterEmblemData* FoundData = CachedSave->SavedCharacters.FindByPredicate(
		[&](const FCharacterEmblemData& Data)
		{
			return Data.CharacterTag == CharacterTag;
		});

	return FoundData ? *FoundData : FCharacterEmblemData();
}

bool UEmblemSaveSubsystem::LoadOrCreateSave(int32 UserIndex)
{
	UObject* Loaded = UGameplayStatics::LoadGameFromSlot("EmblemSlot", UserIndex);

	if (Loaded)
	{
		CachedSave = Cast<UEmblemSaveGame>(Loaded);
	}
	else
	{
		CachedSave = Cast<UEmblemSaveGame>(UGameplayStatics::CreateSaveGameObject(UEmblemSaveGame::StaticClass()));
		if (!CachedSave) return false;

		UGameplayStatics::SaveGameToSlot(CachedSave, "EmblemSlot", UserIndex);
	}

	OnSaveDataLoaded.Broadcast();
	return CachedSave != nullptr;
}

bool UEmblemSaveSubsystem::Save(int32 UserIndex)
{
	if (!CachedSave) return false;

	const bool bSuccess = UGameplayStatics::SaveGameToSlot(CachedSave, "EmblemSlot", UserIndex);
	if (bSuccess)
	{
		NotifyDataChanged();
	}
	return bSuccess;
}
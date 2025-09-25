// Fill out your copyright notice in the Description page of Project Settings.


#include "EmblemSystem/EmblemInventoryComponent.h"

#include "EmblemSystem/EmblemSaveGame.h"
#include "EmblemSystem/EmblemItem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UEmblemInventoryComponent::UEmblemInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEmblemInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FEmblemData UEmblemInventoryComponent::MakeSaveDataFromItem(const UEmblemItem* Item) const
{
	FEmblemData Data;
	Data.UniqueId = Item->UniqueId;
	Data.MergeLevel = Item->MergeLevel;
	Data.MergeCap = Item->MergeCap;
	Data.Stats = Item->Stats;
	Data.Abilities = Item->Abilities;
	return Data;
}

UEmblemItem* UEmblemInventoryComponent::MakeItemFromSaveData(const FEmblemData& Data)
{
	UEmblemItem* NewItem = NewObject<UEmblemItem>(this);
	NewItem->UniqueId = Data.UniqueId;
	NewItem->MergeLevel = Data.MergeLevel;
	NewItem->MergeCap = Data.MergeCap;
	NewItem->Stats = Data.Stats;
	NewItem->Abilities = Data.Abilities;
	return NewItem;
}

void UEmblemInventoryComponent::SaveInventory(UEmblemSaveGame* SaveObject) const
{
	SaveObject->SavedInventory.Empty();
	for (UEmblemItem* Item : Emblems)
	{
		SaveObject->SavedInventory.Add(MakeSaveDataFromItem(Item));
	}
}

void UEmblemInventoryComponent::LoadInventory(const UEmblemSaveGame* SaveObject)
{
	// if (UEmblemSaveGame* Loaded = Cast<UEmblemSaveGame>(UGameplayStatics::LoadGameFromSlot("EmblemSlot", 0)))
 //    {    
 //        for (AActor* Character : AllCharacters)
 //        {
 //            if (UEmblemSlotComponent* SlotComp = Character->FindComponentByClass<UEmblemSlotComponent>())
 //            {
 //            	
 //                FCharacterEmblemData* CharData = Loaded->SavedCharacters.FindByPredicate(
 //                    [&](const FCharacterEmblemData& Data){ return Data.CharacterId == Character->GetUniqueID(); });
 //    
 //                if (CharData)
 //                {
 //                    SlotComp->LoadSlots(*CharData, InventoryComponent->Emblems);
 //                }
 //            }
 //        }
 //    }

	Emblems.Empty();
	for (const FEmblemData& Data : SaveObject->SavedInventory)
	{
		Emblems.Add(MakeItemFromSaveData(Data));
	}
}


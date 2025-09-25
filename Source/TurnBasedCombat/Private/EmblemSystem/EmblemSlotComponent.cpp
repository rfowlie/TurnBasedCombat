// Fill out your copyright notice in the Description page of Project Settings.


#include "EmblemSystem/EmblemSlotComponent.h"
#include "EmblemSystem/EmblemItem.h"


// Sets default values for this component's properties
UEmblemSlotComponent::UEmblemSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEmblemSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEmblemSlotComponent::SaveSlots(FCharacterEmblemData& OutSaveData) const
{
	OutSaveData.EquippedSlots.Empty();

	for (int32 i = 0; i < EquippedEmblems.Num(); ++i)
	{
		FEmblemSlotData Slot;
		Slot.SlotIndex = i;
		Slot.EquippedEmblemId = EquippedEmblems[i] ? EquippedEmblems[i]->UniqueId : FGuid();
		OutSaveData.EquippedSlots.Add(Slot);
	}
}

void UEmblemSlotComponent::LoadSlots(const FCharacterEmblemData& InSaveData, const TArray<UEmblemItem*>& Inventory)
{
	EquippedEmblems.SetNum(InSaveData.EquippedSlots.Num());

	for (const FEmblemSlotData& Slot : InSaveData.EquippedSlots)
	{
		if (!Slot.EquippedEmblemId.IsValid())
		{
			EquippedEmblems[Slot.SlotIndex] = nullptr;
			continue;
		}

		// Find emblem in inventory
		UEmblemItem* Found = *Inventory.FindByPredicate(
			[&](const UEmblemItem* Item) { return Item && Item->UniqueId == Slot.EquippedEmblemId; });

		EquippedEmblems[Slot.SlotIndex] = Found;
	}
}

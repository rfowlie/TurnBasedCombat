// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmblemSystemStructs.h"
#include "EmblemEquipmentComponent.generated.h"


class UEmblemItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDCOMBAT_API UEmblemEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEmblemEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEmblemSlot> EmblemSlots;

	// Equip/unequip API
	bool EquipEmblem(int32 SlotIndex, UEmblemItem* Emblem);
	void UnEquipEmblem(int32 SlotIndex);
};

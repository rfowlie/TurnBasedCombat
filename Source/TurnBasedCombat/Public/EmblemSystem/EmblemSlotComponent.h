// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmblemSystemStructs.h"
#include "EmblemSlotComponent.generated.h"

class UEmblemItem;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDCOMBAT_API UEmblemSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEmblemSlotComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<UEmblemItem*> EquippedEmblems; // same order as slots

	void SaveSlots(FCharacterEmblemData& OutSaveData) const;
	void LoadSlots(const FCharacterEmblemData& InSaveData, const TArray<UEmblemItem*>& Inventory);
};

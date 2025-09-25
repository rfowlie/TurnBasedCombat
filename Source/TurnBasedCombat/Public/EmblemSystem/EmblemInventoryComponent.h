// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmblemSystemStructs.h"
#include "EmblemInventoryComponent.generated.h"

class UEmblemSaveGame;
class UEmblemItem;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDCOMBAT_API UEmblemInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEmblemInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintReadOnly)
	TArray<UEmblemItem*> Emblems;

	FEmblemData MakeSaveDataFromItem(const UEmblemItem* Item) const;
	UEmblemItem* MakeItemFromSaveData(const FEmblemData& Data);

	void SaveInventory(UEmblemSaveGame* SaveObject) const;
	void LoadInventory(const UEmblemSaveGame* SaveObject);
};

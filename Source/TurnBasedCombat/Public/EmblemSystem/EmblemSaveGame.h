// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmblemSystemStructs.h"
#include "GameFramework/SaveGame.h"
#include "EmblemSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UEmblemSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FEmblemData> SavedInventory;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCharacterEmblemData> SavedCharacters; 
};

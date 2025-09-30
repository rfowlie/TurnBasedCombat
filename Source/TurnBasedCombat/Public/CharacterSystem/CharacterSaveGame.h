// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterInventoryManager.h"
#include "CharacterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCharacterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** All characters to save */
	UPROPERTY(BlueprintReadWrite)
	TArray<FCharacterData> SavedCharacters;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GigafireSaveGame.generated.h"


struct FGameplayTag;
struct FUnitLoadData;
struct FUnitSaveData;

/**
 * 
 */
UCLASS()
class GIGAFIRE_API UGigafireSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool GetUnitData(FUnitLoadData& OutLoadData, FGameplayTag UnitTag);
	
	UFUNCTION(BlueprintCallable)
	TArray<FUnitLoadData> GetAllUnitData();

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayTag> GetAllUnitTags();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FUnitSaveData> UnitSaveData;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EmblemSystem/EmblemSystemStructs.h"
#include "EmblemSaveSubsystem.generated.h"


struct FGameplayTag;
class UEmblemSaveGame;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveDataLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmblemSaveDelegate);

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UEmblemSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	UFUNCTION(BlueprintPure, Category = "Save Data")
	bool IsSaveLoaded() const { return CachedSave != nullptr; }
	
	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Save Data")
	FEmblemSaveDelegate OnSaveDataLoaded;

	UPROPERTY(BlueprintAssignable, Category = "Save Data")
	FEmblemSaveDelegate OnSaveDataChanged;
	
	void NotifyDataChanged() const;
	
	// Getters
	UFUNCTION(BlueprintPure, Category = "Save Data|Emblems")
	FEmblemData GetEmblemDataById(const FGuid& EmblemId) const;

	UFUNCTION(BlueprintPure, Category = "Save Data|Characters")
	FCharacterEmblemData GetCharacterDataByTag(const FGameplayTag& CharacterTag) const;

protected:
	// Load or initialize save game
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	bool LoadOrCreateSave(int32 UserIndex);

	// Save current data
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	bool Save(int32 UserIndex);

private:
	UPROPERTY()
	UEmblemSaveGame* CachedSave = nullptr;
};

﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBased_Core_Tags.h"
#include "Grid/GridStructs.h"
#include "Subsystems/WorldSubsystem.h"
#include "Unit/GridUnit.h"
#include "TurnWorldSubsystem.generated.h"


class UGameEventTaskManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnChangedDelegate, int32, TurnNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFactionChangedDelegate, FGameplayTag, FactionTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFactionChangedTaskDelegate, FGameplayTag, FactionTag, UGameEventTaskManager*, TaskManager);

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UTurnWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	bool TurnsActive = false;

	virtual void PostInitialize() override;
	
public:
	UFUNCTION(BlueprintCallable)
	static FGameplayTag GetPlayerFactionTag() { return TAG_TBCore_Faction_Player; }
	
	// flow control	
	UFUNCTION(BlueprintCallable)
	void EnableTurns();
	
	UFUNCTION(BlueprintCallable)
	void DisableTurns();
	
	// events
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FTurnChangedDelegate OnTurnStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FTurnChangedDelegate OnTurnEnd;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedTaskDelegate OnFactionStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedDelegate OnFactionStartPost;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedDelegate OnFactionEnd;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedDelegate OnFactionDefeated;

	UPROPERTY()
	UGameEventTaskManager* OnFactionStartTaskManager = nullptr;

	// turn
	UPROPERTY()
	int32 TurnNumber = 0;
	int32 GetTurnNumber() const;

	// faction
	// UPROPERTY()
	// TArray<FFactionInfo> Factions;
	UPROPERTY()
	TMap<FGameplayTag, FFactionInfo> FactionMap;
	UPROPERTY()
	TArray<FGameplayTag> FactionOrder;	
	UPROPERTY()
	int32 FactionIndex = 0;
	void IncrementFaction();

	TArray<FGameplayTag> GetAllFactions(bool IncludeDefeated = true);
	bool IsFactionValid(FGameplayTag FactionTag) const;
	FGameplayTag GetActiveFaction();
	TArray<AGridUnit*> GetAliveUnitsInFaction(const FGameplayTag FactionTag);
	bool IsFactionActive(FGameplayTag FactionTag);
	bool IsFactionDefeated(FGameplayTag FactionTag);
	UFUNCTION()
	void CheckFactionDefeated(AGridUnit* GridUnit);
	void CheckFactionTurnComplete(const FGameplayTag& FactionTag);
	void SetFactionTurnComplete(const FGameplayTag& FactionTag);
	
	// unit
	bool RegisterGridUnit(AGridUnit* InGridUnit);
	bool CanUnitTakeAction(const AGridUnit* InGridUnit);
	int32 GetRemainingUnitActions();
	AGridUnit* GetNextUnit(AGridUnit* InGridUnit);
	void GetFactionEnemies(AGridUnit* InGridUnit, TArray<AGridUnit*>& EnemyGridUnits);
	void GetAliveFactionEnemies(AGridUnit* InGridUnit, TArray<AGridUnit*>& EnemyGridUnits);
	void SetUnitTurnOver(AGridUnit* InGridUnit);
	// UFUNCTION()
	// void OnCombatEnd(const FCombatPrediction& InCombatPrediction);

protected:

	void DoOnFactionStart();

private:
	UFUNCTION()
	void FactionStartAllTasksComplete();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridStructs.h"
#include "Subsystems/WorldSubsystem.h"
#include "Unit/GridUnit.h"
#include "TurnWorldSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnChangedDelegate, int32, TurnNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFactionChangedDelegate, FGameplayTag, FactionTag);

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UTurnWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// events
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FTurnChangedDelegate OnTurnStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FTurnChangedDelegate OnTurnEnd;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedDelegate OnFactionStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedDelegate OnFactionEnd;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Turn")
	FFactionChangedDelegate OnFactionDefeated;

	// turn
	UPROPERTY()
	int32 TurnNumber = 0;
	int32 GetTurnNumber() const;

	// faction
	UPROPERTY()
	TArray<FFactionInfo> Factions;
	UPROPERTY()
	int32 FactionIndex = 0;
	void IncrementFaction();

	TArray<FGameplayTag> GetAllFactions(bool IncludeDefeated = true);
	bool IsFactionValid(FGameplayTag FactionTag) const;
	FGameplayTag GetActiveFaction();
	bool IsFactionActive(FGameplayTag FactionTag);
	bool IsFactionDefeated(FGameplayTag FactionTag);
	UFUNCTION()
	void CheckFactionDefeated(AGridUnit* GridUnit);
	
	// unit
	bool RegisterGridUnit(AGridUnit* InGridUnit);
	bool CanUnitTakeAction(const AGridUnit* InGridUnit);
	int32 GetRemainingUnitActions();
	
};

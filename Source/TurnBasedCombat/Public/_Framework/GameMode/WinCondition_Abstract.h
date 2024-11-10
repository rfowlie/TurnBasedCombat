// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WinCondition_Abstract.generated.h"

struct FGameplayTag;
class AGridUnit;

UENUM()
enum EWinConditionType
{
	None,
	Win,
	Defeat
};

USTRUCT()
struct FWinConditionPayload
{
	GENERATED_BODY()

	// TODO: what will go in here???
	UPROPERTY()
	TArray<FGameplayTag> FactionsActive;
	UPROPERTY()
	TArray<FGameplayTag> FactionsDefeated;
	
};
/**
 * users will be required to implement one or many of the options below
 * firing the win or defeat events given the correct circumstances
 */
UCLASS(Abstract, EditInlineNew, Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UWinCondition_Abstract : public UObject
{
	GENERATED_BODY()

public:
	// TODO: this is too rigid
	// need to change it to a single function that takes in a payload
	// payload will describe the game state at the snapshot
	// we want to be able to accomodate varied win conditions
	// doing single checks like this are not flexible enough...
	DECLARE_DELEGATE_OneParam(FWinConditionDelegate, EWinConditionType);
	FWinConditionDelegate OnWinCondition;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckGridUnitDefeated(AGridUnit* GridUnit);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckFactionDefeated(FGameplayTag Faction);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckTurnCompleted(int32 Turn);

protected:
	UFUNCTION(BlueprintCallable)
	void CallWinCondition(EWinConditionType WinCondition);
};

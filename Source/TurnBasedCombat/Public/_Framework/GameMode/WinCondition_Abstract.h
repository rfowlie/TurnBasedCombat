// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WinCondition_Abstract.generated.h"

class AGridUnit;

UENUM()
enum EWinConditionType
{
	None,
	Win,
	Defeat
};

/**
 * users will be required to implement one or many of the options below
 * firing the win or defeat events given the correct circumstances
 */
UCLASS(Abstract, Blueprintable)
class TURNBASEDCOMBAT_API UWinCondition_Abstract : public UObject
{
	GENERATED_BODY()

public:	
	DECLARE_DELEGATE_OneParam(WinConditionDelegate, EWinConditionType)
	WinConditionDelegate OnWinCondition;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckGridUnitDefeated(AGridUnit* GridUnit);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckFactionDefeated(FName Faction);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckTurnCompleted(int32 Turn);

};

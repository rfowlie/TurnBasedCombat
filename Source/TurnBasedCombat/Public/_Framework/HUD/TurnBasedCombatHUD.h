// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TurnBasedCombatHUD.generated.h"


struct FGameplayTag;
class UDuelContainer;
class AGridTile;
class UGridProxy;


/**
 * 
 */
UCLASS(Abstract)
class TURNBASEDCOMBAT_API ATurnBasedCombatHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleUI(FGameplayTag UserInterfaceTag, bool bToggle);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUI(FGameplayTag UserInterfaceTag, const UObject* Payload);
	
	// UFUNCTION(BlueprintImplementableEvent)
	// void UpdateGridUnitDisplay(const AGridTile* GridTile);
	//
	// // Combat Inteface ~ start
	// UFUNCTION(BlueprintImplementableEvent)
	// void CombatDisplayToggle(bool On);
	//
	// UFUNCTION(BlueprintImplementableEvent)
	// // void CombatDisplayUpdate(const UGridProxy* InInstigator, const UGridProxy* InTarget, const UGridProxy* InMovingTo);
	// void CombatDisplayUpdate(UCombatProxy* CombatProxy);
	// // Combat Inteface ~ end
protected:
	virtual void BeginPlay() override;
	
};

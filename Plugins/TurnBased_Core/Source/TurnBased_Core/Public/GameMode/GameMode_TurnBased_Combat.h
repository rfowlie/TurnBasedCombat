// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_TurnBased_Combat.generated.h"


class UCombatCalculator_Basic;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatStateChangeDelegate, bool, CombatState);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API AGameMode_TurnBased_Combat : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameMode_TurnBased_Combat();
	
	// flow control
	// FCombatStateChangeDelegate OnCombatStartDelegate;
	// UFUNCTION(BlueprintCallable) 
	// void EnableCombat() const { if (OnCombatStartDelegate.IsBound()) OnCombatStartDelegate.Broadcast(true); }
	// UFUNCTION(BlueprintCallable)
	// void DisableCombat() const { if (OnCombatStartDelegate.IsBound()) OnCombatStartDelegate.Broadcast(false); }
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatCalculator")
	const UCombatCalculator_Basic* GetCombatCalculator();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CombatCalculator")
	TSubclassOf<UCombatCalculator_Basic> CombatCalculatorClass;
	//
	// UPROPERTY(BlueprintReadOnly)
	// UCombatCalculator_Basic* CombatCalculator = nullptr;
};

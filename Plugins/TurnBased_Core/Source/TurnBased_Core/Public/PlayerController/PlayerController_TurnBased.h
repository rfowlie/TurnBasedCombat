﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/CombatData.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_TurnBased.generated.h"


class UUserWidget_ActionOptions;
class UControllerState_Abstract;
class AGridTile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerStateBroadcastDelegate, const FGameplayTag, State);
DECLARE_DELEGATE_OneParam(FControllerStateChangedDelegate, UControllerState_Abstract*)

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API APlayerController_TurnBased : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	APlayerController_TurnBased();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="TurnBased | PlayerController")
	FControllerStateBroadcastDelegate StateBroadcastDelegate;

	// Cursor ~ start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnBased | Cursor")
	TObjectPtr<UStaticMesh> CursorMesh;
	
	UPROPERTY(BlueprintReadOnly, Category = "TurnBased | Cursor")
	TObjectPtr<AActor> Cursor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnBased | Cursor")
	FVector Cursor_ExtraHeight = FVector(0.f, 0.f, 5.f);

protected:
	void CreateCursor();
	
	UPROPERTY(BlueprintReadWrite, Category="TurnBased | Cursor")
	bool CursorVisible = true;

	UFUNCTION()
	void UpdateCursor(AGridTile* GridTile);
	// Cursor ~ end

public:
	// State ~ start
	void EmptyStack();
	
	// walk backwards through array and do exit for each state, then push new state
	UFUNCTION(BlueprintCallable, Category="TurnBased | State")
	void SetBaseState(UControllerState_Abstract* InState);
	
	UFUNCTION(BlueprintCallable, Category="TurnBased | State")
	void PushState(UControllerState_Abstract* InState, bool bDoExit);

	UFUNCTION(BlueprintCallable, Category="TurnBased | State")
	void PopState();

	// useful for 'resetting' state
	// (attack state might want to reset itself but does not want to do a bunch of state saving, resetting logic)
	UFUNCTION(BlueprintCallable, Category="TurnBased | State")
	void PopPushState(UControllerState_Abstract* InState, bool bDoExit);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "TurnBased | State")
	TArray<UControllerState_Abstract*> StateStack;
	// State ~ end

	// Combat ~ start
	UFUNCTION(BlueprintCallable, Category="Combat")
	void OnCombatStart(AGridUnit* InInstigator, AGridUnit* InTarget);
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void OnCombatEnd(const FCombatPrediction& InCombatPrediction);

	UFUNCTION()
	void OnFactionStart(FGameplayTag FactionTag);
	// Combat ~ end
};

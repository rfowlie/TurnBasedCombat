﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_TurnBased.generated.h"

class UWidget_ActionOptions;
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

	UPROPERTY(BlueprintAssignable, Category="TurnBased | PlayerController")
	FControllerStateBroadcastDelegate StateBroadcastDelegate;

	// Cursor ~ start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
	TObjectPtr<UStaticMesh> CursorMesh;
	
	UPROPERTY()
	TObjectPtr<AActor> Cursor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
	FVector Cursor_ExtraHeight = FVector(0.f, 0.f, 5.f);

protected:
	void CreateCursor();
	UFUNCTION()
	void UpdateCursor(AGridTile* GridTile);
	// Cursor ~ end

public:
	// State ~ start
	// walk backwards through array and do exit for each state, then push new state
	UFUNCTION()
	void SetBaseState(UControllerState_Abstract* InState);
	
	UFUNCTION()
	void PushState(UControllerState_Abstract* InState, bool bDoExit);

	UFUNCTION()
	void PopState();
	
protected:
	UPROPERTY()
	TArray<UControllerState_Abstract*> StateStack;
	// State ~ end

public:
	// Widget ~ start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom UI")
	TSubclassOf<UWidget_ActionOptions> ActionWidgetClass;
	// Widget ~ end
	
};

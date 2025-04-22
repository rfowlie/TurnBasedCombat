// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "StackStatePlayerController.generated.h"

class UStackStateBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerControllerStackStateDelegate, const FGameplayTag, State);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class TURNBASEDCOMBAT_API AStackStatePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AStackStatePlayerController();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="PlayerController | StackState")
	FPlayerControllerStackStateDelegate OnStackStateChangedDelegate;

protected:
	void OnStackStateChanged(UStackStateBase* InState) const;
	void EmptyStack();
	void PassInput()

	// simple push functionality
	UFUNCTION(BlueprintCallable, Category="PlayerController | StackState")
	void PushState(UStackStateBase* InState);

	// simple pop functionality
	UFUNCTION(BlueprintCallable, Category="PlayerController | StackState")
	void PopState(UStackStateBase* InState);

	// walk backwards through array and do exit for each state, then push passed state
	UFUNCTION(BlueprintCallable, Category="PlayerController | StackState")
	void SetBaseState(UStackStateBase* InState);

	// pop current state and push passed state
	UFUNCTION(BlueprintCallable, Category="PlayerController | StackState")
	void ReplaceState(UStackStateBase* InState);
	
protected:
	UFUNCTION(BlueprintCallable, Category="PlayerController | StackState")
	TArray<UStackStateBase*> StateStack;
};
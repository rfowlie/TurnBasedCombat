// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GigafireStateMachineBase.generated.h"

class UGigafireStateBase;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API UGigafireStateMachineBase : public UObject
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "Gigafire|State")
	TArray<UGigafireStateBase*> StateStack;
	
	void InitializeState(UGigafireStateBase* InState);
	void DeinitializeState(UGigafireStateBase* InState);
	void EmptyStack();

	UFUNCTION(BlueprintCallable, Category="Gigafire|State")
	UGigafireStateBase* PeakState();
	
	// walk backwards through array and do exit for each state, then push new state
	UFUNCTION(BlueprintCallable, Category="Gigafire|State")
	void SetBaseState(UGigafireStateBase* InState);

	// push a state onto the stack, indicate whether the previous state should stay active or not
	UFUNCTION(BlueprintCallable, Category="Gigafire|State")
	void PushState(UGigafireStateBase* InState, bool bDeinitialize);

	// return to previous state on the stack
	UFUNCTION(BlueprintCallable, Category="Gigafire|State")
	void PopState();

	// useful for 'resetting' state
	// (attack state might want to reset itself but does not want to do a bunch of state saving, resetting logic)
	UFUNCTION(BlueprintCallable, Category="Gigafire|State")
	void PopPushState(UGigafireStateBase* InState, bool bDoExit);
	
};

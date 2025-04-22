// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "StackStateBase.generated.h"

class UStackStateBase;


// set new base state on the stack (clear current stack)
DECLARE_DELEGATE_OneParam(FStackStateDelegate, UStackStateBase*);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StackState_Base);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UStackStateBase : public UObject
{
	GENERATED_BODY()

public:
	// users of this class should subscribe to these to get information about when to act
	FStackStateDelegate OnPushState;
	FStackStateDelegate OnPopState;
	FStackStateDelegate OnSetBaseState;
	FStackStateDelegate OnReplaceState;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual FGameplayTag GetStateTag();
	
	// what the state needs to do to begin working
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void OnEnter(APlayerController* InPlayerController);

	// what the state needs to do clean up
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void OnExit();

	// what the state wants to do when it gets pushed down the stack
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void OnGainFocus();

	// what the state should do when returning but not activating
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void OnLoseFocus();
	
};

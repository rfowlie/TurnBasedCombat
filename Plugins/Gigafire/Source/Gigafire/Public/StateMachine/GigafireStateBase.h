// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Interfaces/GigafireInputHandler.h"
#include "GigafireStateBase.generated.h"

class IGigafirePawnHandler;
class UGigafirePawnHandler;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGigafirePushStateDelegate, UGigafireStateBase*, InState, bool, bDeinitialize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGigafireSetStateDelegate, UGigafireStateBase*, InState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGigafireStateDelegate);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class GIGAFIRE_API UGigafireStateBase : public UObject, public IGigafireInputHandler
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gigafire|State")
	FGameplayTag StateTag;
	
	// setup all the actions for this state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Gigafire|State")
	void Initialize();

	// undo all the actions of this state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Gigafire|State")
	void Deinitialize();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Gigafire|State")
	FGigafireSetStateDelegate OnSetBaseState;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Gigafire|State")
	FGigafirePushStateDelegate OnPushState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Gigafire|State")
	FGigafireStateDelegate OnPopState;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gigafire|State")
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gigafire|State")
	TScriptInterface<UGigafirePawnHandler> PawnHandler;
};

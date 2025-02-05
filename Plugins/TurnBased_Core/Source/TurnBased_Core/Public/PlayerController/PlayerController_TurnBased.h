// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_TurnBased.generated.h"

class UControllerState_Abstract;


DECLARE_DELEGATE_OneParam(FControllerStateChangedDelegate, UControllerState_Abstract*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerStateBroadcastDelegate, const FGameplayTag, State);
/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API APlayerController_TurnBased : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	APlayerController_TurnBased();

	UPROPERTY(BlueprintAssignable, Category="TurnBased | PlayerController")
	FControllerStateBroadcastDelegate StateBroadcastDelegate;


protected:
	UPROPERTY()
	UControllerState_Abstract* State;
	
	UFUNCTION()
	void SetState(UControllerState_Abstract* InState);
};

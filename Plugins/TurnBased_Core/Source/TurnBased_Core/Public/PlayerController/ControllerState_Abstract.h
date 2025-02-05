// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlayerController_TurnBased.h"
#include "UObject/Object.h"
#include "ControllerState_Abstract.generated.h"


class UInputMappingContext;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASED_CORE_API UControllerState_Abstract : public UObject
{
	GENERATED_BODY()

	
public:
	FControllerStateChangedDelegate OnChangedDelegate;
	
	virtual FGameplayTag GetStateTag() const;
	
	UFUNCTION(BlueprintCallable, Category="Controller State")
	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority);
	
	UFUNCTION(BlueprintCallable, Category="Controller State")
	virtual void OnExit(const APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable, Category="Controller State")
	void OnEnable(const APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable, Category="Controller State")
	void OnDisable(const APlayerController* InPlayerController);
	
protected:
	UPROPERTY()
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY()
	int32 InputMappingContextPriority = 1;

	UFUNCTION()
	virtual UInputMappingContext* CreateInputMappingContext(APlayerController* PlayerController);
	
};

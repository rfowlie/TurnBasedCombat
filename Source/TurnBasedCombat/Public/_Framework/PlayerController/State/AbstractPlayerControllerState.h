// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbstractPlayerControllerState.generated.h"


class UInputMappingContext;
class UAbstractEvent;

/**
 * 
 */
UCLASS(Abstract)
class TURNBASEDCOMBAT_API UAbstractPlayerControllerState : public UObject
{
	GENERATED_BODY()

public:
	UAbstractPlayerControllerState();
	
	virtual void CreateInput();
	virtual void EnableInput();
	virtual void DisableInput();

	DECLARE_DELEGATE_OneParam(FOnEventCreate, TArray<UAbstractEvent*> FWorldPartitionEvents);
	FOnEventCreate OnEventCreate;

	///////////////////////////////////////////////////////////////////
	// UFUNCTION()
	// virtual void OnSelect();
	//
	// UFUNCTION()
	// virtual void OnDeselect();
	//
	// // determine when it makes sense to cycle through ally units when in this mode
	// UFUNCTION()
	// virtual void OnCycleUnit();
	
	UFUNCTION()
	void Enter(APlayerController* PlayerController, const int32 InputMappingContextPriority);

	UFUNCTION()
	void Exit(const APlayerController* PlayerController);
	
protected:
	UPROPERTY()
	UInputMappingContext* InputMappingContext = nullptr;

	UFUNCTION()
	virtual UInputMappingContext* SetupInputMappingContext(APlayerController* PlayerController);

	// additional functionality that subclasses wish to add on enter
	UFUNCTION()
	virtual void OnEnter();

	// additional functionality that subclasses wish to add on exit
	UFUNCTION()
	virtual void OnExit();
	
};

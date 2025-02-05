// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "AbstractPlayerControllerState.generated.h"


class UInputMappingContext;
class UAbstractEvent;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Abstract);

UCLASS(Abstract)
class TURNBASEDCOMBAT_API UAbstractPlayerControllerState : public UObject
{
	GENERATED_BODY()

public:
	UAbstractPlayerControllerState();

	virtual FGameplayTag GetStateTag() const;
	
	virtual void CreateInput();
	virtual void EnableInput();
	virtual void DisableInput();

	DECLARE_DELEGATE_OneParam(FOnEventCreate, TArray<UAbstractEvent*> FWorldPartitionEvents);
	FOnEventCreate OnEventCreate;
	
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

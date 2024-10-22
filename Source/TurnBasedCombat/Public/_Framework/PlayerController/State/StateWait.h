// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractPlayerControllerState.h"
#include "StateWait.generated.h"


/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UStateWait : public UAbstractPlayerControllerState
{
	GENERATED_BODY()

public:
	UStateWait();

	// virtual void OnSelect() override;
	// virtual void OnDeselect() override;
	// virtual void OnCycleUnit() override;

protected:	
	virtual void OnEnter() override;
	virtual void OnExit() override;
	
	// UPROPERTY()
	// UInputAction* IA_Select;	
	// UPROPERTY()
	// UInputAction* IA_Deselect;	
	// UPROPERTY()
	// UInputAction* IA_Cycle;
	//
	// virtual UInputMappingContext* SetupInputMappingContext(APlayerController* PlayerController) override;
	
};

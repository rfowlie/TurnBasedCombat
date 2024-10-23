// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/TurnBasedCombatPlayerController.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateMove.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateWait.h"


ATurnBasedCombatPlayerController::ATurnBasedCombatPlayerController()
{
	// enable mouse interaction
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
	
	// Create States
	// UStateWait* StateWait = NewObject<UStateWait>(this);
	UStateWait* StateWait = CreateDefaultSubobject<UStateWait>(TEXT("StateWait"));
	ControllerStates.Add(StateWait);
	// UStateMove* StateMove = NewObject<UStateMove>(this);
	UStateMove* StateMove = CreateDefaultSubobject<UStateMove>(TEXT("StateMove"));
	ControllerStates.Add(StateMove);

	ControllerStateIndex = 0;
	SetState(ControllerStates[ControllerStateIndex]);
	// ControllerStateCurrent = ControllerStates[ControllerStateIndex];
}

void ATurnBasedCombatPlayerController::Initialize(UGridManager* InGridManager)
{
	if (GridManager == nullptr)
	{
		InGridManager = GridManager;
	}
}

void ATurnBasedCombatPlayerController::SetState(UAbstractPlayerControllerState* NewState)
{
	check(NewState);
	// check(ControllerStateCurrent);
	if (ControllerStateCurrent)
	{
		ControllerStateCurrent->Exit(this);
	}
	
	ControllerStateCurrent = NewState;
	ControllerStateCurrent->Enter(this, 2);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_TurnBased.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "PlayerController/ControllerState_Idle.h"


void APlayerController_TurnBased::BeginPlay()
{
	Super::BeginPlay();

	// create initial state idle
	SetState(NewObject<UControllerState_Idle>());
}

APlayerController_TurnBased::APlayerController_TurnBased()
{
	// enable mouse interaction
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
}

void APlayerController_TurnBased::SetState(UControllerState_Abstract* InState)
{
	if (InState == nullptr) { return; }
	if (State)
	{
		State->OnExit(this);
		State->OnChangedDelegate.Unbind();
	}
	State = InState;
	State->OnChangedDelegate.BindUObject(this, &ThisClass::SetState);
	State->OnEnter(this, 2);
	if (StateBroadcastDelegate.IsBound()) { StateBroadcastDelegate.Broadcast(State->GetStateTag()); }
}

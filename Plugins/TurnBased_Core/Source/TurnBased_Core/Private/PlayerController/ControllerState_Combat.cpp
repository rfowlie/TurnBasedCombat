// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Combat.h"


UControllerState_Combat::UControllerState_Combat()
{
}

UControllerState_Combat* UControllerState_Combat::Create()
{
	UControllerState_Combat* Object = NewObject<UControllerState_Combat>();
	return Object;
}

void UControllerState_Combat::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// bind to a callback that indicates combat is over and the player can resume actions...

	// PlayerController->SetBaseState(UControllerState_Combat::Create());
}

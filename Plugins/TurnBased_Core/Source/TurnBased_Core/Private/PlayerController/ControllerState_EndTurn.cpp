// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_EndTurn.h"

UControllerState_EndTurn::UControllerState_EndTurn()
{
}

UControllerState_EndTurn* UControllerState_EndTurn::Create()
{
	UControllerState_EndTurn* Object = NewObject<UControllerState_EndTurn>();
	return Object;
}

void UControllerState_EndTurn::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);
	
}

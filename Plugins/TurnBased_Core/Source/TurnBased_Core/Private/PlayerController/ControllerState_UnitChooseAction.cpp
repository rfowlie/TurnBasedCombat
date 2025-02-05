// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitChooseAction.h"

UControllerState_UnitChooseAction::UControllerState_UnitChooseAction()
{
}

UControllerState_UnitChooseAction* UControllerState_UnitChooseAction::Create()
{
	UControllerState_UnitChooseAction* Object = NewObject<UControllerState_UnitChooseAction>();
	return Object;
}

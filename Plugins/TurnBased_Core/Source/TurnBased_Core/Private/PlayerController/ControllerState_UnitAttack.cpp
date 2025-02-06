// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitAttack.h"
#include "Unit/GridUnit.h"


UControllerState_UnitAttack::UControllerState_UnitAttack()
{
}

UControllerState_UnitAttack* UControllerState_UnitAttack::Create(AGridUnit* InActiveUnit)
{
	auto Object = NewObject<UControllerState_UnitAttack>();
	Object->ActiveUnit = InActiveUnit;
	return Object;
}

void UControllerState_UnitAttack::OnEnter(APlayerController* InPlayerController,
	const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// show selected units half of combat UI
	// show all tiles that can be attacked with equipped weapon
}

void UControllerState_UnitAttack::OnExit()
{
	Super::OnExit();

	// remove combat UI
	// revert all tiles set to attack to original state
}

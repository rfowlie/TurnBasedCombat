// Fill out your copyright notice in the Description page of Project Settings.


#include "_FireEmblemLike/PlayerController/State/FE_StateCombatIdle.h"

void UFE_StateCombatIdle::OnEnter(APlayerController* InPlayerController)
{
	// enable the grid cursor

	// set pawn (camera) to follow the grid cursor
}

void UFE_StateCombatIdle::OnReceiveInput_Select_Implementation()
{
	// check to see if current tile grid cursor is on has a unit
		// push unit selected state
}

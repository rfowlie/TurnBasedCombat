// Fill out your copyright notice in the Description page of Project Settings.

#include "_AbstractFramework/PlayerController/State/StackStateBase.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_StackState_Base, "State.Stack.Base");

FGameplayTag UStackStateBase::GetStateTag_Implementation()
{
	return TAG_StackState_Base;
}

void UStackStateBase::OnEnter_Implementation(APlayerController* InPlayerController)
{
}

void UStackStateBase::OnExit_Implementation()
{
}

void UStackStateBase::OnGainFocus_Implementation()
{
}

void UStackStateBase::OnLoseFocus_Implementation()
{
}

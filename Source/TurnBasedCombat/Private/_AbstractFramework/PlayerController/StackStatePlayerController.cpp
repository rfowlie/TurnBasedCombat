// Fill out your copyright notice in the Description page of Project Settings.


#include "_AbstractFramework/PlayerController/StackStatePlayerController.h"
#include "_AbstractFramework/PlayerController/State/StackStateBase.h"


AStackStatePlayerController::AStackStatePlayerController()
{
}

void AStackStatePlayerController::OnStackStateChanged(UStackStateBase* InState) const
{
	if (InState && OnStackStateChangedDelegate.IsBound())
	{
		OnStackStateChangedDelegate.Broadcast(InState->GetStateTag());
	}
}

void AStackStatePlayerController::EmptyStack()
{
	while(!StateStack.IsEmpty())
	{
		PopState(nullptr);
	}
}

void AStackStatePlayerController::SetBaseState(UStackStateBase* InState)
{
	EmptyStack();	
	PushState(InState);
}

void AStackStatePlayerController::PushState(UStackStateBase* InState)
{
	if (!InState) { return; }
	
	// remove focus
	if (!StateStack.IsEmpty())
	{
		StateStack.Top()->OnLoseFocus();
	}	
	
	InState->OnPushState.BindUObject(this, &ThisClass::PushState);
	InState->OnPopState.BindUObject(this, &ThisClass::PopState);
	InState->OnSetBaseState.BindUObject(this, &ThisClass::SetBaseState);
	InState->OnReplaceState.BindUObject(this, &ThisClass::ReplaceState);
	
	InState->OnEnter(this);
	StateStack.Push(InState);
}

void AStackStatePlayerController::PopState(UStackStateBase* InState)
{
	if (StateStack.IsEmpty()) { return; }
	
	const auto State = StateStack.Pop();
	State->OnPushState.Unbind();
	State->OnPopState.Unbind();
	State->OnSetBaseState.Unbind();
	State->OnReplaceState.Unbind();	
	State->OnExit();

	// reset focus
	if (!StateStack.IsEmpty())
	{
		StateStack.Top()->OnGainFocus();
	}
}

void AStackStatePlayerController::ReplaceState(UStackStateBase* InState)
{
	PopState(InState);
	PushState(InState);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/GigafireStateMachineBase.h"
#include "StateMachine/GigafireStateBase.h"


void UGigafireStateMachineBase::EmptyStack()
{
	while(!StateStack.IsEmpty())
	{
		// don't use pop state as we do not want to reinitialize any states on the stack
		// only make sure they get deinitialized
		UGigafireStateBase* State = StateStack.Pop();
		State->Deinitialize();
	}
}

void UGigafireStateMachineBase::InitializeState(UGigafireStateBase* InState)
{
	StateStack.Push(InState);
	InState->OnSetBaseState.AddUniqueDynamic(this, &ThisClass::SetBaseState);
	InState->OnPushState.AddUniqueDynamic(this, &ThisClass::PushState);
	InState->OnPopState.AddUniqueDynamic(this, &ThisClass::PopState);
	InState->Initialize();
}

void UGigafireStateMachineBase::DeinitializeState(UGigafireStateBase* InState)
{
	if (InState == nullptr) { return; }
	InState->OnSetBaseState.RemoveDynamic(this, &ThisClass::SetBaseState);
	InState->OnPushState.RemoveDynamic(this, &ThisClass::PushState);
	InState->OnPopState.RemoveDynamic(this, &ThisClass::PopState);
	InState->Deinitialize();
}

UGigafireStateBase* UGigafireStateMachineBase::PeakState()
{
	if (StateStack.IsEmpty()) {	return nullptr; }
	return StateStack.Top();
}

void UGigafireStateMachineBase::SetBaseState(UGigafireStateBase* InState)
{
	if (InState == nullptr) { return; }
	EmptyStack();	
	PushState(InState, false);
}

void UGigafireStateMachineBase::PushState(UGigafireStateBase* InState, bool bDeinitialize)
{
	if (InState == nullptr) { return; }
	if (!StateStack.IsEmpty() && bDeinitialize)
	{
		DeinitializeState(StateStack.Top());
	}

	InitializeState(InState);
}

void UGigafireStateMachineBase::PopState()
{
	// if (StateStack.Num() <= 1)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("UGigafireStateMachineBase::PopState - "
	// 			"Attempting to pop from the stack while only %d of states in the stack"), StateStack.Num());
	// 	return;
	// }
	
	auto State = StateStack.Pop();
	State->Deinitialize();
	StateStack.Top()->Initialize();
}

void UGigafireStateMachineBase::PopPushState(UGigafireStateBase* InState, bool bDoExit)
{
	// TODO: pop current state then apply new state on top
	// call OnEnter on bottom state?
}

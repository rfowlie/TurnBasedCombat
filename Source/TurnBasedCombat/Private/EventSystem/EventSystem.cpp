// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/EventSystem/EventSystem.h"
#include "TurnBasedCombat/Public/EventSystem/Events/AbstractEvent.h"


UEventSystem::UEventSystem()
{
}

void UEventSystem::AddEvent(UAbstractEvent* Event)
{
	if (Event)
	{
		Event->OnComplete.BindUObject(this, &ThisClass::CheckQueue);
		EventQueue.Enqueue(Event);
	}
}

void UEventSystem::CheckQueue()
{
	if (IsValid(CurrentEvent) && !CurrentEvent->IsComplete())
	{
		return;
	}
	if (IsValid(CurrentEvent) && CurrentEvent->IsComplete())
	{		
		CurrentEvent = nullptr;
	}
	
	if (EventQueue.IsEmpty())
	{
		if (OnEventsEnd.IsBound()) { OnEventsEnd.Broadcast(); }
	}
	else
	{
		if (OnEventsBegin.IsBound()) { OnEventsBegin.Broadcast(); }
		EventQueue.Dequeue(CurrentEvent);
		if (CurrentEvent->OnStart.IsBound()) { CurrentEvent->OnStart.Execute(); }	
		CurrentEvent->Execute();	
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/GameEventTaskManager.h"

void UGameEventTaskManager::RegisterTask(UObject* Object)
{
	if (Object && !TaskSet.Contains(Object))
	{
		TaskSet.Add(Object);
		UE_LOG(LogTemp, Log, TEXT("Registered: %s"), *Object->GetName());
	}
}

void UGameEventTaskManager::UnregisterTask(UObject* Object)
{
	if (Object && TaskSet.Contains(Object))
	{
		TaskSet.Remove(Object);
		UE_LOG(LogTemp, Log, TEXT("Completed: %s"), *Object->GetName());
		CheckTasksComplete();
	}
}

void UGameEventTaskManager::EnqueueTask(UGameEventTaskContainer* InTaskContainer)
{
	if (InTaskContainer) { TaskQueue.Enqueue(InTaskContainer); }
}

void UGameEventTaskManager::StartNextTaskInQueue()
{
	if (TaskQueue.IsEmpty()) { CheckTasksComplete(); return; }
	TaskQueue.Dequeue(CurrentTaskContainer);
	CurrentTaskContainer->OnTaskCompleted.AddDynamic(this, &ThisClass::StartNextTaskInQueue);
	CurrentTaskContainer->Task.Execute();
}

void UGameEventTaskManager::CheckTasksComplete() const
{
	if (TaskSet.IsEmpty() && TaskQueue.IsEmpty())
	{
		OnAllTasksCompleted.Execute();
	}
}

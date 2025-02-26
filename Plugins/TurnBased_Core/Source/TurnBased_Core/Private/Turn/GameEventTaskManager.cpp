// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/GameEventTaskManager.h"
#include "Turn/GameEventTask_Async.h"


UGameEventTaskManager::UGameEventTaskManager()
{
}

UGameEventTaskManager* UGameEventTaskManager::Create()
{
	UGameEventTaskManager* Object = NewObject<UGameEventTaskManager>();
	return Object;
}

void UGameEventTaskManager::RegisterAsyncTask(UGameEventTask_Async* InAsyncTask, int32 Phase)
{
	// do not allow registering of tasks while executing
	if (bAsyncTasksInitiated) { return; }
	
	// do not add tasks that are empty
	if (!InAsyncTask->OnExecuteDelegate.ExecuteIfBound()) { return; }
	
	Phase = FMath::Max(Phase, 0);
	if (!AsyncTaskMap.Contains(Phase))
	{
		AsyncTaskMap.Add(Phase, FAsyncTaskArray());
	}
	
	AsyncTaskMap[Phase].Tasks.AddUnique(InAsyncTask);
}

void UGameEventTaskManager::InitiateAsyncTasks()
{
	if (bAsyncTasksInitiated) { return; }
	bAsyncTasksInitiated = true;
	AsyncTaskMap.GetKeys(PhaseOrder);
	PhaseOrder.Sort();

	if (PhaseOrder.IsEmpty())
	{
		if (OnAllTasksCompleted.IsBound()) { OnAllTasksCompleted.Execute(); }
		return;
	}

	PhaseIndex = -1;
	ExecuteNextPhase();
}

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

void UGameEventTaskManager::ExecuteNextPhase()
{
	PhaseIndex++;
	if (!PhaseOrder.IsValidIndex(PhaseIndex))
	{
		// finished all tasks
		if (OnAllTasksCompleted.IsBound()) { OnAllTasksCompleted.Execute(); }
		return;
	}
	
	for (const auto AsyncTask : AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks)
	{
		AsyncTask->OnComplete.AddUniqueDynamic(this, &ThisClass::UGameEventTaskManager::CheckPhaseComplete);
		AsyncTask->OnExecuteDelegate.Execute();
	}
}

void UGameEventTaskManager::CheckPhaseComplete(UGameEventTask_Async* AsyncTask)
{
	// if (!PhaseOrder.IsValidIndex(PhaseIndex))
	// {
	// 	// finished all tasks
	// 	if (OnAllTasksCompleted.IsBound()) { OnAllTasksCompleted.Execute(); }
	// 	return;
	// }
	
	if (AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.IsEmpty() || !AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.Contains(AsyncTask))
	{
		UE_LOG(LogTemp, Error, TEXT("Async Task firing out of place!!"))
	}

	AsyncTask->OnComplete.RemoveAll(this);
	AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.Remove(AsyncTask);
	if (AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.IsEmpty())
	{
		ExecuteNextPhase();
	}
}

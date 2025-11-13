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
	if (!InAsyncTask->OnExecuteDelegate.IsBound()) { return; }
	
	Phase = FMath::Max(Phase, 0);
	if (!AsyncTaskMap.Contains(Phase))
	{
		AsyncTaskMap.Add(Phase, FAsyncTaskArray());
	}
	
	AsyncTaskMap[Phase].Tasks.AddUnique(InAsyncTask);
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

void UGameEventTaskManager::InitiateAllTasks()
{
	if (OnManagerBegin.IsBound()) { OnManagerBegin.Broadcast(); }
	InitiateAsyncTasks();
}

void UGameEventTaskManager::InitiateAsyncTasks()
{
	if (bAsyncTasksInitiated) { return; }
	bAsyncTasksInitiated = true;
	AsyncTaskMap.GetKeys(PhaseOrder);
	PhaseOrder.Sort();

	if (PhaseOrder.IsEmpty())
	{
		if (OnManagerComplete.IsBound()) { OnManagerComplete.Broadcast(); }
		return;
	}

	PhaseIndex = -1;
	ExecuteNextPhase();
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
		if (OnManagerComplete.IsBound()) { OnManagerComplete.Broadcast(); }
	}
}

void UGameEventTaskManager::ExecuteNextPhase()
{
	PhaseIndex++;
	// if at end of array completely finished
	if (!PhaseOrder.IsValidIndex(PhaseIndex))
	{
		if (OnManagerComplete.IsBound()) { OnManagerComplete.Broadcast(); }
		return;
	}

	// bind to all tasks in current phase and initiate them
	for (const auto AsyncTask : AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks)
	{
		AsyncTask->OnComplete.AddUniqueDynamic(this, &ThisClass::UGameEventTaskManager::CheckPhaseComplete);
		AsyncTask->OnExecuteDelegate.Execute();
	}
}

void UGameEventTaskManager::CheckPhaseComplete(UGameEventTask_Async* AsyncTask)
{
	// flag if task does not belong to current phase
	if (AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.IsEmpty() || !AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.Contains(AsyncTask))
	{
		UE_LOG(LogTemp, Error, TEXT("UGameEventTaskManager::CheckPhaseComplete - Async task firing out of place!!"))
	}

	// remove task from array
	AsyncTask->OnComplete.RemoveAll(this);
	AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.Remove(AsyncTask);

	// check if array is empty, signal to move onto next phase
	if (AsyncTaskMap[PhaseOrder[PhaseIndex]].Tasks.IsEmpty())
	{
		ExecuteNextPhase();
	}
}

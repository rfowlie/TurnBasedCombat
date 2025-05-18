// Fill out your copyright notice in the Description page of Project Settings.


#include "_Basic/Async/GenericAsyncNode.h"

UGenericAsyncNode* UGenericAsyncNode::GenericAsyncNode()
{
	UGenericAsyncNode* Node = NewObject<UGenericAsyncNode>();
	return Node;
}

void UGenericAsyncNode::Activate()
{
	(new FAutoDeleteAsyncTask<FGenericNonAbandonableTask>(this))->StartBackgroundTask();
}

FGenericNonAbandonableTask::FGenericNonAbandonableTask(UGenericAsyncNode* GenericAsyncNode)
{
	CallingObject = GenericAsyncNode;
}

FGenericNonAbandonableTask::~FGenericNonAbandonableTask()
{
	CallingObject->OnComplete.Broadcast();
	CallingObject->SetReadyToDestroy();
}

void FGenericNonAbandonableTask::DoWork()
{
	CallingObject->Task.Broadcast();
}

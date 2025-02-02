// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventTaskContainer.h"
#include "UObject/Object.h"
#include "GameEventTaskManager.generated.h"


DECLARE_DYNAMIC_DELEGATE(FGameEventTaskDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTasksCompleted);

/**
 * we want this manager to allow the sequential execution of registered code,
 * or allow a variety of objets to register and unregister when they are done their work
 * objects can then sub to the on all tasks complete delegate to be notified when everything
 * is completed in this manager
 */
UCLASS(BlueprintType)
class TURNBASED_CORE_API UGameEventTaskManager : public UObject
{
	GENERATED_BODY()

public:
	// Event triggered when all registered objects have completed
	UPROPERTY()
	FGameEventTaskDelegate OnAllTasksCompleted;

	// Add an object to the wait list
	UFUNCTION(BlueprintCallable, Category = "Turn Completion")
	void RegisterTask(UObject* Object);

	// Mark an object as completed
	UFUNCTION(BlueprintCallable, Category = "Turn Completion")
	void UnregisterTask(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Turn Completion")
	void EnqueueTask(UGameEventTaskContainer* InTaskContainer);
	
	UFUNCTION()
	void StartNextTaskInQueue();

private:
	// The set of objects that need to complete before moving forward
	UPROPERTY()
	TSet<UObject*> TaskSet;
	
	TQueue<UGameEventTaskContainer*> TaskQueue;

	UPROPERTY()
	UGameEventTaskContainer* CurrentTaskContainer = nullptr;

	// Check if all objects are done
	void CheckTasksComplete() const;
	
};

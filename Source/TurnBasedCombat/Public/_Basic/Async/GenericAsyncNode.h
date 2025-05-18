// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GenericAsyncNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskOutput);

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UGenericAsyncNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTaskOutput Task;

	UPROPERTY(BlueprintAssignable)
	FTaskOutput OnComplete;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UGenericAsyncNode* GenericAsyncNode();

	virtual void Activate() override;
	
};

//============================================================================================

class FGenericNonAbandonableTask : public FNonAbandonableTask
{
public:
	
	FGenericNonAbandonableTask(UGenericAsyncNode* GenericAsyncNode);

	~FGenericNonAbandonableTask();

	//UE5 needs this
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FGenericNonAbandonableTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	UGenericAsyncNode* CallingObject = nullptr;

	void DoWork();

};
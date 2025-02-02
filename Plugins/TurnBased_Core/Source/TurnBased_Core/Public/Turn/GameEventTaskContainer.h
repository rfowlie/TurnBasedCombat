// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameEventTaskContainer.generated.h"

DECLARE_DYNAMIC_DELEGATE(FGameEventTaskContainerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEventTaskContainerMulticast);

/**
 * pass a delegate task to run and a delegate callback to listen too
 */
UCLASS(BlueprintType)
class TURNBASED_CORE_API UGameEventTaskContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameEventTaskContainerDelegate Task;

	UPROPERTY()
	FGameEventTaskContainerMulticast OnTaskCompleted;
};

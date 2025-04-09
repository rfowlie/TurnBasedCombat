// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ObjectPool.generated.h"


/*
 * use object design pattern to facilitate AActor object pooling
 */
UCLASS(BlueprintType)
class CHIMERAOBJECTPOOLING_API UObjectPool : public UObject
{
	GENERATED_BODY()

	UObjectPool();
	
public:
	UFUNCTION(BlueprintCallable)
	static UObjectPool* Create(UWorld* InWorldContext, TSubclassOf<AActor> InActorClass, int32 AmortizedValue = 1);
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetObjects(int32 Amount = 1);

private:
	UPROPERTY()
	UWorld* WorldContext = nullptr;
	
	UPROPERTY()
	TSubclassOf<AActor> ActorClass;

	UPROPERTY()
	TArray<AActor*> Pool;

	UFUNCTION()
	AActor* CreatePooledActor();
	
};



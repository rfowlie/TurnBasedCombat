// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolWorldSubsystem.generated.h"

class UObjectPool;

/*
 * provide a single touch point to use object pooling without requiring
 * objects to own and manage the object pool themselves
 */
UCLASS()
class CHIMERAOBJECTPOOLING_API UObjectPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AActor* GetObjectByClass(TSubclassOf<AActor> ActorClass);
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetObjectsByClass(TSubclassOf<AActor> ActorClass, int32 Amount);

	UFUNCTION(BlueprintCallable)
	AActor* GetObjectByActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetObjectsByActor(AActor* InActor, int32 Amount);
	
private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, UObjectPool*> ClassPoolMap;

};
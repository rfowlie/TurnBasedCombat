// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectPool.h"
#include "ObjectPoolable.h"


UObjectPool::UObjectPool()
{
}

UObjectPool* UObjectPool::Create(
	UWorld* InWorldContext, const TSubclassOf<AActor> InActorClass, const int32 AmortizedValue)
{
	// need world context to spawn actors
	if (!InWorldContext)
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPool::Create - World Context nullptr"));
		return nullptr;
	}
	
	// do not create object pool if class does not implement interface
	if (!InActorClass->ClassDefaultObject->Implements<UObjectPoolable>())
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPool::Create - Actor class does not implement UObjectPoolable"));
		return nullptr;
	}
	
	UObjectPool* Obj = NewObject<UObjectPool>();
	Obj->WorldContext = InWorldContext;
	Obj->ActorClass = InActorClass;

	// initialize desired number of actors to start
	for (int32 i = 0; i < AmortizedValue; i++)
	{
		Obj->CreatePooledActor();
	}
	
	return Obj;
}

TArray<AActor*> UObjectPool::GetObjects(int32 Amount)
{
	Amount = FMath::Max(1, Amount);
	
	TArray<AActor*> OutActors;
	for (const auto Actor : Pool)
	{
		if (!IObjectPoolable::Execute_IsActive(Actor))
		{
			OutActors.Add(Actor);
			if (OutActors.Num() >= Amount)
			{
				return OutActors;
			}
		}
	}
	
	// insufficient actors remaining, create more and add to pool
	const int32 Remaining = Amount - OutActors.Num();
	for (int32 i = 0; i < Remaining; i++)
	{
		if (AActor* Actor = CreatePooledActor())
		{
			OutActors.Add(Actor);
		}
	}	

	return OutActors;
}

AActor* UObjectPool::CreatePooledActor()
{
	if (!WorldContext) return nullptr;
	AActor* Actor = WorldContext->SpawnActor(ActorClass);
	if (Actor)
	{		
		// ensure actor is deactivated pool is created
		IObjectPoolable::Execute_Deactivate(Actor);
		Pool.Add(Actor);
	}

	return Actor;
}


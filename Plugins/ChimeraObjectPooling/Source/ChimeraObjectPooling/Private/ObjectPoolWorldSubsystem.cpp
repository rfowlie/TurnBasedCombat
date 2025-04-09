// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectPoolWorldSubsystem.h"
#include "ObjectPool.h"


AActor* UObjectPoolWorldSubsystem::GetObjectByClass(TSubclassOf<AActor> ActorClass)
{
	if (!ActorClass) { return nullptr; }
	
	// if pool does not exist create
	if (!ClassPoolMap.Contains(ActorClass))
	{
		if (UObjectPool* ObjectPool = UObjectPool::Create(GetWorld(), ActorClass, 1))
		{
			ClassPoolMap.Add(ActorClass, ObjectPool);
			
		}
	}
	// check pool exists
	if (ClassPoolMap.Contains(ActorClass))
	{
		if (TArray<AActor*> OutActors = ClassPoolMap[ActorClass]->GetObjects(1); OutActors.Num() > 0)
		{
			return OutActors[0];
		}
	}
	
	return nullptr;
}

TArray<AActor*> UObjectPoolWorldSubsystem::GetObjectsByClass(TSubclassOf<AActor> ActorClass, int32 Amount)
{
	TArray<AActor*> OutActors;
	if (!ActorClass) return OutActors;
	Amount = FMath::Max(Amount, 1);
	
	// if pool does not exist create
	if (!ClassPoolMap.Contains(ActorClass))
	{
		if (UObjectPool* ObjectPool = UObjectPool::Create(GetWorld(), ActorClass, Amount))
		{
			ClassPoolMap.Add(ActorClass, ObjectPool);
		}
	}

	// pool creation failed
	if (!ClassPoolMap.Contains(ActorClass)) { return OutActors; }
	
	OutActors = ClassPoolMap[ActorClass]->GetObjects(Amount);
	return OutActors;
}

AActor* UObjectPoolWorldSubsystem::GetObjectByActor(AActor* InActor)
{
	if (IsValid(InActor); UClass* Class = InActor->GetClass())
	{
		return GetObjectByClass(Class);
	}

	return nullptr;
}

TArray<AActor*> UObjectPoolWorldSubsystem::GetObjectsByActor(AActor* InActor, int32 Amount)
{
	if (IsValid(InActor); UClass* Class = InActor->GetClass())
	{
		return GetObjectsByClass(Class, Amount);
	}

	return TArray<AActor*>();
}

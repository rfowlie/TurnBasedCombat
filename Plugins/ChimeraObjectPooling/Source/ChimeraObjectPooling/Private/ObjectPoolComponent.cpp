// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"
#include "ObjectPool.h"


void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	ObjectPool = NewObject<UObjectPool>(this);
	ObjectPool = UObjectPool::Create(GetWorld(), ActorClass, AmortizedValue);
	if (ObjectPool == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::BeginPlay - Could not create object pool"));
	}
}

TArray<AActor*> UObjectPoolComponent::GetObjects(const int32 Amount)
{
	if (ObjectPool == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::GetObjects - object pool is nullptr"));
		return TArray<AActor*>();
	}
	
	return ObjectPool->GetObjects(Amount);
}

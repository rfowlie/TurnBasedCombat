// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

class UObjectPool;

/*
 * wrapper for UObjectPool so users can attach and set values in blueprint
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHIMERAOBJECTPOOLING_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AmortizedValue = 1;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetObjects(int32 Amount = 1);
	
protected:
	UPROPERTY()
	UObjectPool* ObjectPool;

};

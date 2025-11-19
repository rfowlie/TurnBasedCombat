// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GridUnitBase.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UNREALGRIDMECHANICS_API AGridUnitBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FGameplayTag GetFactionTag();
};

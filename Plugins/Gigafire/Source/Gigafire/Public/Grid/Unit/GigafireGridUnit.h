// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GigafireGridUnit.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API AGigafireGridUnit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGigafireGridUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);
};


USTRUCT(BlueprintType)
struct FGigafireGridUnitArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AGigafireGridUnit*> GridUnits;
	
};
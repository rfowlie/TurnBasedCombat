// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridInfo.generated.h"



/*
 * should exist in every level with a grid, will provide the world subsystem
 * with all relevant dynamic information
 * */

UCLASS()
class TURNBASEDGRIDSYSTEMS_API AGridInfo : public AActor
{
	GENERATED_BODY()

public:
	AGridInfo();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	int32 TileSize = 200;
};

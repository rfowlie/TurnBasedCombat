// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManagerActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelManagerEventDelegate, bool, bSuccess);

/*
 * every level should contain one of these, and it should be used to store all required or relevant information
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API ALevelManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelManagerActor();

	// return if player won level or lost
	UPROPERTY(BlueprintAssignable)
	FLevelManagerEventDelegate OnLevelComplete;
};

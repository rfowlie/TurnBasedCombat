// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelWinConditionActor.generated.h"

/*
 * Each level will be expected to contain one of these
 * it will do whatever it needs to find actors and organize conditions
 * it will call the IWinConditionGameMode->SetWinCondition() passing itself so the gamemode
 * can bind to its callbacks
 * OR should the gamemode look for all actors of this type and sub to their callbacks?
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelWinConditionDelegate, bool, Success);

UCLASS()
class TURNBASEDCOMBAT_API ALevelWinConditionActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelWinConditionActor();

	FLevelWinConditionDelegate OnWinConditionComplete;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupWinCondition();

	UFUNCTION(BlueprintImplementableEvent)
	void CheckWinCondition();

	void CheckWinCondition_Internal();
};

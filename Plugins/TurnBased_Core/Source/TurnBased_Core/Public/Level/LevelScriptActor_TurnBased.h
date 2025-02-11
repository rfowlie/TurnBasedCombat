// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptActor_TurnBased.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelScriptActorEventDelegate, bool, bSuccess);

UCLASS(BlueprintType)
class TURNBASED_CORE_API ALevelScriptActor_TurnBased : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelScriptActor_TurnBased();
	
	UPROPERTY(BlueprintAssignable)
	FLevelScriptActorEventDelegate OnLevelComplete;
};

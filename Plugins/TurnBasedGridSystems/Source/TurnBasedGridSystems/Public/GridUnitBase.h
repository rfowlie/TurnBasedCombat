// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridUnitBase.generated.h"

struct FGridStatePayload;


UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDGRIDSYSTEMS_API AGridUnitBase : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:

	// allow each tile to control how it updates it's state according to the game
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGridState(const FGridStatePayload Payload);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridEventPayload.generated.h"

class UGridProxy;
/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASEDCOMBAT_API UGridEventPayload : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UGridProxy* MoveToLocation;
};

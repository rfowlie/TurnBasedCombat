// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridEventPayload_Move.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASEDCOMBAT_API UGridEventPayload_Move : public UObject
{
	GENERATED_BODY()

public:
	static UGridEventPayload_Move* CreatePayload(const FVector& Location);

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector Location;
	
private:
	UGridEventPayload_Move();
	
};

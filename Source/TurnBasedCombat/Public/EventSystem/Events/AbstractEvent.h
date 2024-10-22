// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbstractEvent.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UAbstractEvent : public UObject
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnStart;
	FSimpleDelegate OnComplete;
	
	virtual void Execute() {}

	bool IsComplete() const { return Complete; }
	
protected:
	bool Complete = false;
};

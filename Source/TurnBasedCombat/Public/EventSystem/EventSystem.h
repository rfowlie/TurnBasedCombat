// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EventSystem.generated.h"


class UAbstractEvent;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UEventSystem : public UObject
{
	GENERATED_BODY()

public:
	UEventSystem();

	DECLARE_EVENT(UEventSystem, FEventNotify)
	FEventNotify OnEventsBegin;
	FEventNotify OnEventsEnd;

	void AddEvent(UAbstractEvent* Event);
	void CheckQueue();
	
protected:
	TQueue<UAbstractEvent*> EventQueue = TQueue<UAbstractEvent*>();
	
	UPROPERTY()
	UAbstractEvent* CurrentEvent = nullptr;

	
};

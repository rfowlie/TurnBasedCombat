// Fill out your copyright notice in the Description page of Project Settings.


#include "GridState/GridStateManagerComponent.h"
#include "GridState/GridStateChangeHandler.h"


void UGridStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// initialize default handler
	if (DefaultGridStateHandler)
	{
		UObject* Obj = NewObject<UObject>(this, DefaultGridStateHandler);
		SetGridStateHandler(Obj);
	}
}

UGridStateManagerComponent::UGridStateManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UGridStateManagerComponent::SetGridStateHandler(UObject* InGridStateChangeHandler)
{
	// ensure implements correct interface
	if (IsValid(InGridStateChangeHandler) && InGridStateChangeHandler->Implements<UGridStateChangeHandler>())
	{
		GridStateChangeHandler = TScriptInterface<IGridStateChangeHandler>(InGridStateChangeHandler);
	}
}


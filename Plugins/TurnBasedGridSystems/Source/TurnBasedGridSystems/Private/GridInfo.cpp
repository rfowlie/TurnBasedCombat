// Fill out your copyright notice in the Description page of Project Settings.


#include "GridInfo.h"

#include "GridWorldSubsystem.h"


AGridInfo::AGridInfo()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGridInfo::BeginPlay()
{
	Super::BeginPlay();

	if (UGridWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Subsystem->RegisterGridInfo(this);
	}
}



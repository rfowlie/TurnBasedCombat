// Fill out your copyright notice in the Description page of Project Settings.


#include "GridUnitBase.h"
#include "GridWorldSubsystem.h"


void AGridUnitBase::BeginPlay()
{
	Super::BeginPlay();

	if (UGridWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Subsystem->RegisterGridUnit(this);
	}
}
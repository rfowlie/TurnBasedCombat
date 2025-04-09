// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTileBase.h"
#include "GridWorldSubsystem.h"


void AGridTileBase::BeginPlay()
{
	Super::BeginPlay();

	if (UGridWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Subsystem->RegisterGridTile(this);
	}
}


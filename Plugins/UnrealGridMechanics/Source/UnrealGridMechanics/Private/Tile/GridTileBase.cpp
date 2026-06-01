// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GridTileBase.h"

#include "Subsystem/GridTrackerSubsystem.h"


AGridTileBase::AGridTileBase()
{
}

void AGridTileBase::BeginPlay()
{
	Super::BeginPlay();

	// world subsystem should always know about this
	if (UGridTrackerSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridTrackerSubsystem>())
	{
		Subsystem->RegisterGridTile(this);
	}
}

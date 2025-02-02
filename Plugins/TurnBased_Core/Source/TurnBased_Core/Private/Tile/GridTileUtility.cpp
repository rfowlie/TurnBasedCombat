// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GridTileUtility.h"
#include "Tile/GridTile.h"
#include "Grid/GridWorldSubsystem.h"


bool UGridTileUtility::RegisterGridTile(AGridTile* InGridTile)
{
	if (InGridTile == nullptr) { return false; }

	// pass this grid unit to all subsystems that require unit registration			
	if (UGridWorldSubsystem* Subsystem = InGridTile->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Subsystem->RegisterGridTile(InGridTile);
	}
	
	return true;
}

bool UGridTileUtility::UnregisterGridTile(AGridTile* InGridTile)
{
	return false;
}

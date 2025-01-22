// Fill out your copyright notice in the Description page of Project Settings.


#include "_Framework/TBC_InfoWorldSubsystem.h"

void UTBC_InfoWorldSubsystem::SetGridTileHovered(AGridTile* InGridTile)
{
	GridTileHoveredCurrent = InGridTile;
	if (OnGridTileHovered.IsBound()) { OnGridTileHovered.Broadcast(GridTileHoveredCurrent); }
}

void UTBC_InfoWorldSubsystem::SetGridTileSelected(AGridTile* InGridTile)
{
	GridTileSelectedCurrent = InGridTile;
	if (OnGridTileSelected.IsBound()) { OnGridTileSelected.Broadcast(GridTileSelectedCurrent); }
}

void UTBC_InfoWorldSubsystem::SetGridUnitHovered(AGridUnit* InGridUnit)
{
	GridUnitHoveredCurrent = InGridUnit;
	if (OnGridUnitHovered.IsBound()) { OnGridUnitHovered.Broadcast(GridUnitHoveredCurrent); }
}

void UTBC_InfoWorldSubsystem::SetGridUnitSelected(AGridUnit* InGridUnit)
{
	GridUnitSelectedCurrent = InGridUnit;
	if (OnGridUnitSelected.IsBound()) { OnGridUnitSelected.Broadcast(GridUnitSelectedCurrent); }
}

void UTBC_InfoWorldSubsystem::SetPlayerControllerMode(FGameplayTag InGameplayTag)
{
	PlayerControllerMode = InGameplayTag;
	if (OnPlayerControllerUpdateMode.IsBound()) { OnPlayerControllerUpdateMode.Broadcast(PlayerControllerMode); }
}

void UTBC_InfoWorldSubsystem::SetPlayerControllerPhase(FGameplayTag InGameplayTag)
{
	PlayerControllerPhase = InGameplayTag;
	if (OnPlayerControllerUpdatePhase.IsBound()) { OnPlayerControllerUpdatePhase.Broadcast(PlayerControllerPhase); }
}
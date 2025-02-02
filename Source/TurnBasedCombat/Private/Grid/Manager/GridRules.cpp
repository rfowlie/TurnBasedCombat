// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Manager/GridRules.h"
#include "Grid/Manager/GridManager.h"
#include "Tile/GridTile.h"


bool UGridRules::UnitCanMoveOnTile(UGridManager* GridManager, AGridUnit* InUnit, AGridTile* StartTile, AGridTile* TargetTile)
{
	// TODO: setup rules for what units can move on to what tiles, if units can move through tiles occupied by other units
	return GridManager->GetGridUnitOnTile(TargetTile) == nullptr;
}

int32 UGridRules::CalculateTerrainCost(UGridManager* GridManager, AGridUnit* InUnit, AGridTile* StartTile, AGridTile* TargetTile)
{
	// TODO: allow setup of custom logic for terrain and units as well as unit through unit custom cost
	return TargetTile->GetMovementCost();
}

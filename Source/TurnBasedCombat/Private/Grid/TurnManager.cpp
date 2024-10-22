// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/TurnManager.h"
#include "Grid/Unit/GridUnit.h"


UTurnManager::UTurnManager()
{
	
}

void UTurnManager::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (IsValid(GridUnit) && !GridUnitsMap.Contains(GridUnit))
	{
		GridUnitsMap.Add(GridUnit, false);
		Factions.AddUnique(GridUnit->GetFaction());
	}
}

void UTurnManager::UnregisterGridUnit(AGridUnit* GridUnit)
{
}

bool UTurnManager::CanTakeTurn()
{
	return false;
}

void UTurnManager::SetNextFaction()
{
	FactionIndex = (FactionIndex + 1 + Factions.Num()) % Factions.Num();
	
	TArray<AGridUnit*> GridUnits;
	GridUnitsMap.GetKeys(GridUnits);
	for (const AGridUnit* GridUnit : GridUnits)
	{
		GridUnit->GetFaction() == Factions[FactionIndex] ?
			GridUnitsMap[GridUnit] = true : GridUnitsMap[GridUnit] = false;
	}
}

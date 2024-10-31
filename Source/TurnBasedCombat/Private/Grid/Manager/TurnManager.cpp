// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Manager/TurnManager.h"
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
		FactionsMap.Add(GridUnit->GetFaction(), true);
	}
}

void UTurnManager::UnregisterGridUnit(AGridUnit* GridUnit)
{
	if (IsValid(GridUnit) && GridUnitsMap.Contains(GridUnit))
	{
		GridUnitsMap.Remove(GridUnit);
	}
}

FName UTurnManager::GetCurrentFaction() const
{
	return Factions[FactionIndex];
}

bool UTurnManager::CanTakeTurn(AGridUnit* GridUnit)
{
	if (!CheckGridUnit(GridUnit)) { return false; }

	return GridUnitsMap[GridUnit];
}

void UTurnManager::UpdateGridUnitMoved(AGridUnit* GridUnit)
{
	if (!CheckGridUnit(GridUnit)) { return; }

	// set unit cannot move
	GridUnitsMap[GridUnit] = false;

	// check if any units can move, if not signal new turn should start
	for (const auto Pair : GridUnitsMap)
	{
		if (Pair.Value) { return; }
	}

	SetNextFaction();
	if (OnTurnFinish.IsBound()) { OnTurnFinish.Broadcast(GetCurrentFaction()); }
}

void UTurnManager::UpdateGridUnitAttacked(AGridUnit* GridUnit)
{
	if (!CheckGridUnit(GridUnit)) { return; }

	// set unit cannot move
	GridUnitsMap[GridUnit] = false;

	// check if any units can move, if not signal new turn should start
	for (const auto Pair : GridUnitsMap)
	{
		if (Pair.Value) { return; }
	}

	SetNextFaction();
	if (OnTurnFinish.IsBound()) { OnTurnFinish.Broadcast(GetCurrentFaction()); }
}

void UTurnManager::UpdateGridUnitKilled(AGridUnit* GridUnit)
{
	if (!CheckGridUnit(GridUnit)) { return; }

	GridUnitsMap.Remove(GridUnit);

	// update faction map
	TArray<AGridUnit*> GridUnits;
	GridUnitsMap.GetKeys(GridUnits);
	for (const AGridUnit* Unit : GridUnits)
	{
		FactionsMap[Unit->GetFaction()] = true;
	}
	
	// TODO: this will need to be more robust to support different scenarios
	// check to see if only units of one faction remain	
	FName RemainingFaction = GridUnits[0]->GetFaction();
	for (const AGridUnit* Unit : GridUnits)
	{
		// two or more teams present keep playings
		if (Unit->GetFaction() != RemainingFaction) { return; }
	}

	// all grid units had same faction therefore game over
	if (OnFactionWin.IsBound()) { OnFactionWin.Execute(RemainingFaction); }
}

void UTurnManager::IncrementFaction()
{
	FactionIndex = (FactionIndex + 1 + Factions.Num()) % Factions.Num();
}

void UTurnManager::SetNextFaction()
{
	IncrementFaction();
	// check to see if all units of faction destroyed, skip turn???
	while(!FactionsMap[Factions[FactionIndex]]) { IncrementFaction(); }

	UE_LOG(LogTemp, Log, TEXT("Faction Start Turn: %s"), *Factions[FactionIndex].ToString());
	
	TArray<AGridUnit*> GridUnits;
	GridUnitsMap.GetKeys(GridUnits);
	for (const AGridUnit* GridUnit : GridUnits)
	{
		GridUnit->GetFaction() == Factions[FactionIndex] ?
			GridUnitsMap[GridUnit] = true : GridUnitsMap[GridUnit] = false;
	}
}

bool UTurnManager::CheckGridUnit(const AGridUnit* GridUnit) const
{
	if (!GridUnit)
	{
		UE_LOG(LogTemp, Error, TEXT("grid unit passed to turn manager nullptr..."))
		return false;
	}
	if (!GridUnitsMap.Contains(GridUnit))
	{
		UE_LOG(LogTemp, Error, TEXT("grid unit not registered with turn manager..."))
		return false;
	}

	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Manager/TurnManager.h"
#include "Grid/Unit/GridUnit.h"


UTurnManager::UTurnManager()
{
	
}

void UTurnManager::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }

	FName FactionName = GridUnit->GetFaction();
	bool FactionFound = false;
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Name == FactionName)
		{
			FactionInfo.GridUnits.Add(GridUnit);
			FactionFound = true;
			break;
		}
	}
	if (!FactionFound)
	{
		FFactionInfo FactionInfo;
		FactionInfo.Name = FactionName;
		FactionInfo.GridUnits.Add(GridUnit);
		Factions.Add(FactionInfo);	
	}

	// make adjustments when a unit is defeated
	GridUnit->OnDefeat.AddDynamic(this, &ThisClass::CheckFactionDefeated);
}

void UTurnManager::UnregisterGridUnit(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }
	
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Name == GridUnit->GetFaction())
		{
			FactionInfo.GridUnits.Remove(GridUnit);
			break;
		}
	}
}

FName UTurnManager::GetCurrentFaction() const
{
	return Factions[FactionIndex].Name;
}

bool UTurnManager::CanTakeTurn(AGridUnit* GridUnit)
{
	return Factions[FactionIndex].GridUnits.Contains(GridUnit);
}

void UTurnManager::UpdateGridUnitActionTaken(AGridUnit* GridUnit)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Name == GridUnit->GetFaction())
		{
			FactionInfo.GridUnits[GridUnit] = false;

			// check if any units can move, if not signal new turn should start
			if (!FactionInfo.CanUnitsMove())
			{
				if (OnTurnFinish.IsBound()) { OnTurnFinish.Broadcast(GetCurrentFaction()); }
				SetNextFaction();				
			}			
			
			break;
		}
	}
}


void UTurnManager::IncrementFaction()
{
	FactionIndex = (FactionIndex + 1 + Factions.Num()) % Factions.Num();
}

void UTurnManager::CheckFactionDefeated(AGridUnit* GridUnit)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Name == GridUnit->GetFaction())
		{
			if (FactionInfo.IsFactionDefeated())
			{
				if (OnFactionDefeated.IsBound()) { OnFactionDefeated.Execute(GridUnit->GetFaction()); }
				break;
			}
		}
	}
}

void UTurnManager::SetNextFaction()
{
	IncrementFaction();
	// check to see if all units of faction destroyed, skip turn???
	while(Factions[FactionIndex].IsFactionDefeated()) { IncrementFaction(); }

	UE_LOG(LogTemp, Log, TEXT("Faction Start Turn: %s"), *Factions[FactionIndex].Name.ToString());
	
	Factions[FactionIndex].ActivateUnits();
}

void UTurnManager::GetActiveFactions(TArray<FName>& ActiveFactions)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (!FactionInfo.IsFactionDefeated())
		{
			ActiveFactions.Add(FactionInfo.Name);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Manager/TurnManager.h"
#include "Grid/Unit/GridUnit.h"


UTurnManager::UTurnManager()
{
	
}

void UTurnManager::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }

	FGameplayTag FactionTag = GridUnit->Execute_GetFaction(GridUnit);
	bool FactionFound = false;
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Tag == FactionTag)
		{
			FactionInfo.GridUnits.Add(GridUnit);
			FactionFound = true;
			break;
		}
	}
	if (!FactionFound)
	{
		FFactionInfo FactionInfo;
		FactionInfo.Tag = FactionTag;
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
		if (FactionInfo.Tag == GridUnit->Execute_GetFaction(GridUnit))
		{
			FactionInfo.GridUnits.Remove(GridUnit);
			break;
		}
	}
}

// TODO: seems like move/attack event firing before factions can even be updated???
FGameplayTag UTurnManager::GetCurrentFaction() const
{
	// TODO: this sometimes fires before it can be updated
	if (FactionIndex < 0) { return FGameplayTag(); }
	return Factions[FactionIndex].Tag;
}

bool UTurnManager::CanTakeTurn(AGridUnit* GridUnit)
{
	return Factions[FactionIndex].GridUnits.Contains(GridUnit);
}

void UTurnManager::UpdateGridUnitActionTaken(AGridUnit* GridUnit)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Tag == GridUnit->Execute_GetFaction(GridUnit))
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
		if (FactionInfo.Tag == GridUnit->Execute_GetFaction(GridUnit))
		{
			if (FactionInfo.IsFactionDefeated())
			{
				if (OnFactionDefeated.IsBound()) { OnFactionDefeated.Execute(GridUnit->Execute_GetFaction(GridUnit)); }
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

	UE_LOG(LogTemp, Log, TEXT("Faction Start Turn: %s"), *Factions[FactionIndex].Tag.ToString());
	
	Factions[FactionIndex].ActivateUnits();
}

void UTurnManager::GetActiveFactions(TArray<FGameplayTag>& ActiveFactions)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (!FactionInfo.IsFactionDefeated())
		{
			ActiveFactions.Add(FactionInfo.Tag);
		}
	}
}

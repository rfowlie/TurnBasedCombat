// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Manager/TurnManager.h"
#include "Grid/Unit/GridUnit.h"
#include "_Framework/TBC_InfoWorldSubsystem.h"


UTurnManager::UTurnManager()
{
	
}

void UTurnManager::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }

	FGameplayTag FactionTag = GridUnit->Execute_GetFaction(GridUnit);
	bool FactionFound = false;
	for (FFactionInfo& FactionInfo : Factions)
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

void UTurnManager::UsedTurn(const AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }
	for (FFactionInfo& FactionInfo : Factions)
	{
		if (FactionInfo.GridUnits.Contains(GridUnit))
		{
			FactionInfo.GridUnits[GridUnit] = false;
		}
	}
	
	CheckFactionTurnComplete();
}

void UTurnManager::CheckFactionTurnComplete()
{
	if (!Factions[FactionIndex].CanUnitsMove())
	{
		if (OnTurnFinish.IsBound()) { OnTurnFinish.Broadcast(GetCurrentFaction()); }
		SetNextFaction();		
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

	// for now just broadcast this through the world subsystem
	if (UTBC_InfoWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UTBC_InfoWorldSubsystem>())
	{
		if (Subsystem->OnFactionChanged.IsBound()) { Subsystem->OnFactionChanged.Broadcast(Factions[FactionIndex].Tag); }
	}
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

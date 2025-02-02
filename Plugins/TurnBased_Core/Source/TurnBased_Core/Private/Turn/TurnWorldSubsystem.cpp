// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/TurnWorldSubsystem.h"

#include "Grid/GridStructs.h"


int32 UTurnWorldSubsystem::GetTurnNumber() const
{
	return TurnNumber;
}

void UTurnWorldSubsystem::IncrementFaction()
{
	if (Factions.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Turn Manager - Increment Faction: no factions registered"));
		return;
	}
	
	if (FactionIndex + 1 < Factions.Num())
	{
		// not all factions finished for this turns
		if (OnFactionEnd.IsBound()) { OnFactionEnd.Broadcast(Factions[FactionIndex].Tag); }
		do {
			FactionIndex++;
		} while (Factions[FactionIndex].IsFactionDefeated());
		if (OnFactionStart.IsBound()) { OnFactionStart.Broadcast(Factions[FactionIndex].Tag); }
		Factions[FactionIndex].ActivateUnits();
	}
	else
	{
		// all factions have finished for this turn, move to next turn
		if (OnFactionEnd.IsBound()) { OnFactionEnd.Broadcast(Factions[FactionIndex].Tag); }
		if (OnTurnEnd.IsBound()) { OnTurnEnd.Broadcast(TurnNumber); }
		TurnNumber += 1;
		if (OnTurnStart.IsBound()) { OnTurnStart.Broadcast(TurnNumber); }
		FactionIndex = 0;
		while (Factions[FactionIndex].IsFactionDefeated()) { FactionIndex++; }
		if (OnFactionStart.IsBound()) { OnFactionStart.Broadcast(Factions[FactionIndex].Tag); }
		Factions[FactionIndex].ActivateUnits();
	}
}

TArray<FGameplayTag> UTurnWorldSubsystem::GetAllFactions(const bool IncludeDefeated)
{
	TArray<FGameplayTag> OutFactions;
	for (FFactionInfo FactionInfo : Factions)
	{
		if (IncludeDefeated || !FactionInfo.IsFactionDefeated())
		{
			OutFactions.Add(FactionInfo.Tag);
		}
	}

	return OutFactions;
}

bool UTurnWorldSubsystem::IsFactionValid(const FGameplayTag FactionTag) const
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Tag.MatchesTagExact(FactionTag))
		{
			return true;
		}
	}

	return false;
}

FGameplayTag UTurnWorldSubsystem::GetActiveFaction()
{
	if (Factions.IsEmpty()) { return FGameplayTag(); }
	return Factions[FactionIndex].Tag;
}

bool UTurnWorldSubsystem::IsFactionActive(const FGameplayTag FactionTag)
{
	if (Factions.IsEmpty()) { return false; }
	return Factions[FactionIndex].Tag.MatchesTagExact(FactionTag);
}

bool UTurnWorldSubsystem::IsFactionDefeated(const FGameplayTag FactionTag)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Tag.MatchesTagExact(FactionTag))
		{
			return FactionInfo.IsFactionDefeated();
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Turn World Subsystem - Is Faction Defeated: faction not registered"));
	return false;
}

void UTurnWorldSubsystem::CheckFactionDefeated(AGridUnit* GridUnit)
{
	for (FFactionInfo FactionInfo : Factions)
	{
		if (FactionInfo.Tag.MatchesTagExact(GridUnit->Execute_GetFaction(GridUnit)))
		{
			if (FactionInfo.IsFactionDefeated())
			{
				if (OnFactionDefeated.IsBound()) { OnFactionDefeated.Broadcast(FactionInfo.Tag); }
				break;
			}
		}
	}
}

bool UTurnWorldSubsystem::RegisterGridUnit(AGridUnit* InGridUnit)
{
	if (!IsValid(InGridUnit)) { return false; }

	FGameplayTag FactionTag = InGridUnit->Execute_GetFaction(InGridUnit);
	bool FactionFound = false;
	for (FFactionInfo& FactionInfo : Factions)
	{
		if (FactionInfo.Tag.MatchesTagExact(FactionTag))
		{
			FactionInfo.GridUnits.Add(InGridUnit);
			FactionFound = true;
			break;
		}
	}
	if (!FactionFound)
	{
		FFactionInfo FactionInfo;
		FactionInfo.Tag = FactionTag;
		FactionInfo.GridUnits.Add(InGridUnit);
		Factions.Add(FactionInfo);	
	}

	// check faction defeated when unit defeated
	InGridUnit->OnDefeat.AddDynamic(this, &ThisClass::CheckFactionDefeated);
	return true;
}

bool UTurnWorldSubsystem::CanUnitTakeAction(const AGridUnit* InGridUnit)
{
	// if not units factions turn, then should not be able to take action
	FGameplayTag InGridUnitFaction = InGridUnit->Execute_GetFaction(InGridUnit);
	if (!Factions[FactionIndex].Tag.MatchesTagExact(InGridUnitFaction)) { return false; }
	if (!Factions[FactionIndex].GridUnits.Contains(InGridUnit))
	{
		UE_LOG(LogTemp, Error, TEXT("Turn World Subsystem - Can Unit Take Action: unit not registered"));
		return false;
	}
	return Factions[FactionIndex].GridUnits[InGridUnit];
}

int32 UTurnWorldSubsystem::GetRemainingUnitActions()
{
	return Factions[FactionIndex].GetActiveUnitsCount();
}

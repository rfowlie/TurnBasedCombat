// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn/TurnWorldSubsystem.h"

#include "Grid/GridStructs.h"


int32 UTurnWorldSubsystem::GetTurnNumber() const
{
	return TurnNumber;
}

void UTurnWorldSubsystem::IncrementFaction()
{
	if (FactionMap.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Turn Manager - Increment Faction: no factions registered"));
		return;
	}
	
	if (FactionIndex + 1 < FactionOrder.Num())
	{
		// not all factions finished for this turns
		if (OnFactionEnd.IsBound()) { OnFactionEnd.Broadcast(FactionOrder[FactionIndex]); }
		do {
			FactionIndex++;
		} while (FactionMap[FactionOrder[FactionIndex]].IsFactionDefeated());
		if (OnFactionStart.IsBound()) { OnFactionStart.Broadcast(FactionOrder[FactionIndex]); }
		FactionMap[FactionOrder[FactionIndex]].ActivateUnits();
	}
	else
	{
		// all factions have finished for this turn, move to next turn
		if (OnFactionEnd.IsBound()) { OnFactionEnd.Broadcast(FactionOrder[FactionIndex]); }
		if (OnTurnEnd.IsBound()) { OnTurnEnd.Broadcast(TurnNumber); }
		TurnNumber += 1;
		if (OnTurnStart.IsBound()) { OnTurnStart.Broadcast(TurnNumber); }
		FactionIndex = 0;
		while (FactionMap[FactionOrder[FactionIndex]].IsFactionDefeated()) { FactionIndex++; }
		if (OnFactionStart.IsBound()) { OnFactionStart.Broadcast(FactionOrder[FactionIndex]); }
		FactionMap[FactionOrder[FactionIndex]].ActivateUnits();
	}
}

TArray<FGameplayTag> UTurnWorldSubsystem::GetAllFactions(const bool IncludeDefeated)
{
	TArray<FGameplayTag> OutFactions;
	for (auto Pair : FactionMap)
	{
		if (IncludeDefeated || !Pair.Value.IsFactionDefeated())
		{
			OutFactions.Add(Pair.Key);
		}
	}

	return OutFactions;
}

bool UTurnWorldSubsystem::IsFactionValid(const FGameplayTag FactionTag) const
{
	return FactionMap.Contains(FactionTag);
}

FGameplayTag UTurnWorldSubsystem::GetActiveFaction()
{
	return FactionOrder[FactionIndex];
}

bool UTurnWorldSubsystem::IsFactionActive(const FGameplayTag FactionTag)
{
	return FactionOrder[FactionIndex].MatchesTagExact(FactionTag);
}

bool UTurnWorldSubsystem::IsFactionDefeated(const FGameplayTag FactionTag)
{
	return FactionMap[FactionTag].IsFactionDefeated();
}

void UTurnWorldSubsystem::CheckFactionDefeated(AGridUnit* GridUnit)
{
	for (auto Pair : FactionMap)
	{
		if (Pair.Value.GridUnits.Contains(GridUnit))
		{
			if (OnFactionDefeated.IsBound()) { OnFactionDefeated.Broadcast(Pair.Key); }
			break;
		}
	}
}

bool UTurnWorldSubsystem::RegisterGridUnit(AGridUnit* InGridUnit)
{
	if (!IsValid(InGridUnit)) { return false; }

	// check faction defeated when unit defeated
	InGridUnit->OnDefeat.AddDynamic(this, &ThisClass::CheckFactionDefeated);
	
	FGameplayTag FactionTag = InGridUnit->Execute_GetFaction(InGridUnit);
	if (FactionMap.Contains(FactionTag))
	{
		FactionMap[FactionTag].GridUnits.Add(InGridUnit);
	}
	else
	{
		FFactionInfo FactionInfo;
		FactionInfo.GridUnits.Add(InGridUnit);
		FactionMap.Add(FactionTag, FactionInfo);
	}
	
	return true;
}

bool UTurnWorldSubsystem::CanUnitTakeAction(const AGridUnit* InGridUnit)
{
	// if not units factions turn, then should not be able to take action
	FGameplayTag FactionTag = InGridUnit->Execute_GetFaction(InGridUnit);
	if (!FactionOrder[FactionIndex].MatchesTagExact(FactionTag)) { return false; }
	return FactionMap[FactionTag].GridUnits[InGridUnit];
}

int32 UTurnWorldSubsystem::GetRemainingUnitActions()
{
	return FactionMap[FactionOrder[FactionIndex]].GetActiveUnitsCount();
}

// this should be a generic next unit
AGridUnit* UTurnWorldSubsystem::GetNextUnit(AGridUnit* InGridUnit)
{
	const FGameplayTag FactionTag = InGridUnit->Execute_GetFaction(InGridUnit);
	return FactionMap[FactionTag].GetNextUnit(InGridUnit);
	// // if unit that cannot take turn is passed in, return first unit of faction that is active
	// if (!FactionOrder[FactionIndex].MatchesTagExact(FactionTag))
	// {
	// 	TArray<AGridUnit*> Keys;
	// 	FactionMap[FactionOrder[FactionIndex]].GridUnits.GetKeys(Keys);
	// 	return Keys[0];
	// }
	//
	// return FactionMap[FactionOrder[FactionIndex]].GetNextUnit(InGridUnit);
}

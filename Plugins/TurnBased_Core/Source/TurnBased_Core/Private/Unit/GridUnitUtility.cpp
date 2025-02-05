// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/GridUnitUtility.h"
#include "Grid/GridWorldSubsystem.h"
#include "Turn/TurnWorldSubsystem.h"


bool UGridUnitUtility::RegisterGridUnit(AGridUnit* InGridUnit)
{
	if (InGridUnit == nullptr) { return false; }

	// pass this grid unit to all subsystems that require unit registration			
	if (UGridWorldSubsystem* Subsystem = InGridUnit->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Subsystem->RegisterGridUnit(InGridUnit);
	}
	if (UTurnWorldSubsystem* Subsystem = InGridUnit->GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
	{
		Subsystem->RegisterGridUnit(InGridUnit);
	}
	
	return true;
}

bool UGridUnitUtility::UnregisterGridUnit(AGridUnit* InGridUnit)
{
	return false;
}

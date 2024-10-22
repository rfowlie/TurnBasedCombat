// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/Unit/GridUnit.h"

#include "TurnBasedCombat/Public/Item/Weapon.h"


AGridUnit::AGridUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGridUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridUnit::BeginPlay()
{
	Super::BeginPlay();	
}

int32 AGridUnit::GetAvailableMovement() const
{
	return UnitStatsSnapshot.Movement;
}

FName AGridUnit::GetFaction() const
{
	return FName("Empty");
}

TArray<UWeapon*> AGridUnit::GetEquippedWeapons() const
{
	return EquippedWeapons;
}

TSet<int32> AGridUnit::GetWeaponRanges() const
{
	TSet<int32> OutValues;
	for (const auto Weapon : EquippedWeapons)
	{
		OutValues.Add(Weapon->GetRange());
	}

	return OutValues;
}

FUnitStatsSnapshot AGridUnit::GetSnapshot() const
{
	return UnitStatsSnapshot;
}

void AGridUnit::UpdateStats(const FUnitStatsSnapshot& StatAdjustments)
{
	// TODO: this is so sketch
	UnitStatsSnapshot += StatAdjustments;
}

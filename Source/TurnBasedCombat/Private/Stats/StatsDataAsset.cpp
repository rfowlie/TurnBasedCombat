// Fill out your copyright notice in the Description page of Project Settings.



#include "TurnBasedCombat/Public/Stats/StatsDataAsset.h"
#include "TurnBasedCombat/Public/Stats/StatsHelpers.h"


void UStatsDataAsset::GetStats(FUnitStatsSnapshot& UnitStats, const int32 Level) const
{
	const float LevelSample = FMath::Clamp(Level, UStatsHelpers::GetMinLevel(), UStatsHelpers::GetMaxLevel()) / UStatsHelpers::GetMaxLevelFloat();

	UnitStats.Health = GetStatValue(Health, LevelSample);
	UnitStats.Mana = GetStatValue(Mana, LevelSample);
	UnitStats.Strength = GetStatValue(Strength, LevelSample);
	UnitStats.Defence = GetStatValue(Defence, LevelSample);
	UnitStats.Speed = GetStatValue(Speed, LevelSample);
	UnitStats.Skill = GetStatValue(Skill, LevelSample);
	UnitStats.Magic = GetStatValue(Magic, LevelSample);
	UnitStats.Resistance = GetStatValue(Resistance, LevelSample);
	UnitStats.Luck = GetStatValue(Luck, LevelSample);
	UnitStats.Movement = GetStatValue(Movement, LevelSample);
	UnitStats.Constitution = GetStatValue(Constitution, LevelSample);
}

float UStatsDataAsset::GetStatValue(const FStatRange& StatRange, const float Level) const
{
	// TODO: for some reason this is getting called on editor startup, if any information missing then crashes
	float Lerp = StatRange.GrowthCurve ? StatRange.GrowthCurve->GetFloatValue(Level) : Level;
	return FMath::Lerp(StatRange.StatMin, StatRange.StatMax, Lerp);
	
	// if (StatRange.GrowthCurve == nullptr)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Growth Curve Missing"));
	// 	return FMath::Lerp(StatRange.StatMin, StatRange.StatMax, Level);
	// }
	//
	// return FMath::Lerp(StatRange.StatMin, StatRange.StatMax, StatRange.GrowthCurve->GetFloatValue(Level));
}

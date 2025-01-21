// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Unit/GridUnitAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Stats/StatsDataAsset.h"



UGridUnitAttributeSet::UGridUnitAttributeSet()
{
}

#pragma region Overrides
void UGridUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGridUnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UGridUnitAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UGridUnitAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool UGridUnitAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UGridUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// clamp health to zero and max health, ensure max health set
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetHealthMax()));

		// broadcast when health drops to zero, but only once
		if (!IsDefeated && GetHealth() <= 0)
		{
			IsDefeated = true;
			if (OnHealthZero.IsBound()) { OnHealthZero.Broadcast(); }
		}
	}
}
#pragma endregion 

#pragma region OnRep
void UGridUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Health, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, HealthMax, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Strength(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Strength, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Skill(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Skill, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Speed(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Speed, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Luck(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Luck, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Defence(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Defence, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Resistance(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Resistance, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Movement(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Movement, PreviousValue);
}

void UGridUnitAttributeSet::OnRep_Constitution(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Constitution, PreviousValue);
}

void UGridUnitAttributeSet::InitializeAttributesFromStatsDataAsset(
	UStatsDataAsset* StatsDataAsset, const int32 Level)
{
	if (!StatsDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize stats from data asset"));
		return;
	}
	
	StatsDataAsset->GetStats(StatsSnapshot, Level);

	// TODO: this is temporary till we understand which stats we want???
	// or is this the start of a good polymorphic setup...
	InitHealthMax(StatsSnapshot.Health);
	InitHealth(StatsSnapshot.Health);
	InitStrength(StatsSnapshot.Strength);
	// InitSkill(StatsSnapshot.)
	InitSpeed(StatsSnapshot.Speed);
	InitLuck(StatsSnapshot.Luck);
	InitDefence(StatsSnapshot.Defence);
	InitResistance(StatsSnapshot.Resistance);
	InitMovement(StatsSnapshot.Movement);
	// InitConstitution(StatsSnapshot.)
}

#pragma endregion 

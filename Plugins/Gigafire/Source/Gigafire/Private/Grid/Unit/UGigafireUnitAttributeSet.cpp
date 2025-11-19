// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Unit/UGigafireUnitAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GigafireSaveGame.h"
#include "Net/UnrealNetwork.h"


UGigafireUnitAttributeSet::UGigafireUnitAttributeSet()
{
}

#pragma region Overrides
void UGigafireUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGigafireUnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UGigafireUnitAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UGigafireUnitAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool UGigafireUnitAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UGigafireUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
void UGigafireUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Health, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, HealthMax, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Strength(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Strength, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Skill(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Skill, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Speed(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Speed, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Luck(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Luck, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Defence(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Defence, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Resistance(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Resistance, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Movement(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Movement, PreviousValue);
}

void UGigafireUnitAttributeSet::OnRep_Constitution(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGigafireUnitAttributeSet, Constitution, PreviousValue);
}

void UGigafireUnitAttributeSet::InitializeAttributeSet(const FGigafireUnitAttributes& InAttributes)
{
	InitHealthMax(InAttributes.Health);
	InitHealth(InAttributes.Health);
	InitStrength(InAttributes.Strength);
	InitSkill(InAttributes.Skill);
	InitSpeed(InAttributes.Speed);
	InitLuck(InAttributes.Luck);
	InitDefence(InAttributes.Defence);
	InitResistance(InAttributes.Resistance);
	InitMovement(InAttributes.Movement);
	InitConstitution(InAttributes.Constitution);
}

bool UGigafireUnitAttributeSet::IsUnitDefeated() const
{
	return IsDefeated;
}

#pragma endregion 

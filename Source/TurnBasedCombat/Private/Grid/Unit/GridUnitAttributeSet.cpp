// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Unit/GridUnitAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"



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

	// clamp health to zero and max health
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetHealthMax()));
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

void UGridUnitAttributeSet::OnRep_Defence(const FGameplayAttributeData& PreviousValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGridUnitAttributeSet, Defence, PreviousValue);
}
#pragma endregion 

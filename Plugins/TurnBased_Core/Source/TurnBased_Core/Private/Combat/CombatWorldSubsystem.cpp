// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatWorldSubsystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TurnBased_Core_Tags.h"
#include "Combat/CombatCalculator_Basic.h"
#include "GameMode/GameMode_TurnBased_Combat.h"
#include "Unit/GridUnit.h"


void UCombatWorldSubsystem::InitiateCombat(AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit)
{
	if (!IsValid(InInstigatorUnit) || !IsValid(InTargetUnit)) { return; }
	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	if (!GameMode) { return; }
	CombatCalculator = GameMode->GetCombatCalculator();
	if (!CombatCalculator) { return; }
	CombatCalculator->GetCombatOutcome(CombatOutcome, InInstigatorUnit, InTargetUnit);
	if (CombatOutcome.CombatOrder.IsEmpty()) { return; }
	if (OnCombatStart.IsBound()) { OnCombatStart.Broadcast(InInstigatorUnit, InTargetUnit); }
	DoCombatTurn();
}

void UCombatWorldSubsystem::DoCombatTurn()
{
	ActiveUnit = CombatOutcome.CombatOrder[0];
	CombatOutcome.CombatOrder.RemoveAt(0);

	DelegateHandle = ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);

	// send gameplay event
	FGameplayEventData EventData;
	EventData.Instigator = ActiveUnit;
	EventData.Target = ActiveUnit == CombatOutcome.Instigator ? CombatOutcome.Target : CombatOutcome.Instigator;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, TAG_Event_Grid_Attack, EventData);
}

void UCombatWorldSubsystem::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
{
	// remove delegate otherwise odd behaviour
	if (InGameplayAbility->GetAvatarActorFromActorInfo() == ActiveUnit)
	{
		ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.Remove(DelegateHandle);
	}

	// only end when attack order finished
	if (!CombatOutcome.CombatOrder.IsEmpty() &&
		 CombatOutcome.Instigator->GetHealth() > 0 && CombatOutcome.Target->GetHealth() > 0)
	{
		DoCombatTurn();
	}
	else
	{
		if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(InstigatorUnit, TargetUnit); }
	}
}

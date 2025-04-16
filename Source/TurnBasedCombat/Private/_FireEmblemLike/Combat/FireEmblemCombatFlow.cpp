// Fill out your copyright notice in the Description page of Project Settings.


#include "_FireEmblemLike/Combat/FireEmblemCombatFlow.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "_FireEmblemLike/Combat/FireEmblemCombatGamplayAbilityPayload.h"


void UFireEmblemCombatFlow::InitiateCombat(const FFireEmblemCombatPrediction& InCombatPrediction)
{
	if (bCombatActive)
	{
		UE_LOG(LogTemp, Log, TEXT("Initiate Combat while active combat: %s"), *InCombatPrediction.GetString());
		return;
	}

	if (InCombatPrediction.CombatOrder.IsEmpty()) { return; }

	CombatPrediction = InCombatPrediction;
	CombatOrderIndex = 0;

	// TODO: for now... just move units here...
	CombatPrediction.CombatInformation.InstigatorUnit->SetActorLocation(CombatPrediction.CombatInformation.InstigatorTile->GetPlacementLocation());	
	bCombatActive = true;
	if (OnCombatStartDelegate.IsBound()) { OnCombatStartDelegate.ExecuteIfBound(CombatPrediction); }
	
	SendCombatEventToNextUnit();
}

void UFireEmblemCombatFlow::SendCombatEventToNextUnit()
{
	UE_LOG(LogTemp, Log, TEXT("Sending CombatEvent to NextUnit"));
	
	ActiveUnit = CombatPrediction.CombatOrder[CombatOrderIndex];
	ActiveUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(this, &ThisClass::CacheGameplayAbility);
	ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::CheckGameplayAbility);
	
	// send gameplay event
	FGameplayEventData EventData;
	EventData.Instigator = ActiveUnit;
	EventData.Target = ActiveUnit == CombatPrediction.CombatInformation.InstigatorUnit ?
		CombatPrediction.CombatInformation.TargetUnit : CombatPrediction.CombatInformation.InstigatorUnit;
	
	UFireEmblemCombatGamplayAbilityPayload* EventWrapper = NewObject<UFireEmblemCombatGamplayAbilityPayload>();
	EventWrapper->InstigatorSnapShotAdvanced = ActiveUnit == CombatPrediction.CombatInformation.InstigatorUnit ?
		CombatPrediction.TargetSnapshotAdvanced : CombatPrediction.InstigatorSnapShotAdvanced;
	EventData.OptionalObject = EventWrapper;
	
	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, TAG_Event_Grid_Attack, EventData);
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Grid.Attack"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, EventTag, EventData);

}

void UFireEmblemCombatFlow::CacheGameplayAbility(UGameplayAbility* InGameplayAbility)
{
	/*
	 * a bit janky, we are hoping that the first gameplay ability triggered on the unit we are
	 * monitoring will be the correct one, could easily not be...
	 */
	ActiveGameplayAbility = InGameplayAbility;
}

void UFireEmblemCombatFlow::CheckGameplayAbility(UGameplayAbility* InGameplayAbility)
{
	if (ActiveGameplayAbility == InGameplayAbility)
	{
		ActiveUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.RemoveAll(this);
		ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
		CombatOrderIndex++;

		if (CombatPrediction.CombatOrder.IsValidIndex(CombatOrderIndex) &&
			CombatPrediction.CombatInformation.InstigatorUnit->GetHealth() > 0 &&
			CombatPrediction.CombatInformation.TargetUnit->GetHealth() > 0)
		{
			SendCombatEventToNextUnit();
		}
		else
		{
			CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
			CombatPrediction.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
			
			// should call this every time a combat finishes, not just when all units finished (AI)
			bCombatActive = false;
			if (OnCombatEndDelegate.IsBound()) { OnCombatEndDelegate.ExecuteIfBound(CombatPrediction); }
		}
	}
}
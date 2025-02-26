// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatWorldSubsystem.h"
#include "TurnBased_Core_Tags.h"
#include "Unit/GridUnit.h"
#include "AI/ActionEvaluator_Combat.h"
#include "Combat/CombatEventWrapper.h"
#include "AbilitySystemBlueprintLibrary.h"


DEFINE_LOG_CATEGORY(Log_Combat);

void UCombatWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
	
}

void UCombatWorldSubsystem::InitiateCombat(const FCombatPrediction& InCombatPrediction)
{
	if (bCombatActive)
	{
		UE_LOG(Log_Combat, Error, TEXT("Initiate Combat while active combat: %s"), *InCombatPrediction.GetString());
		return;
	}

	if (InCombatPrediction.CombatOrder.IsEmpty()) { return; }

	CombatPrediction = InCombatPrediction;
	CombatOrderIndex = 0;
	
	// DEBUG
	float GameTime = GetWorld()->GetTimeSeconds();
	UE_LOG(Log_Combat, Log, TEXT("Game Time: %f"), GameTime);
	UE_LOG(Log_Combat, Warning, TEXT("INITIATECOMBAT - %s"), *CombatPrediction.GetString());	

	// TODO: for now... just move units here...
	CombatPrediction.CombatInformation.InstigatorUnit->SetActorLocation(CombatPrediction.CombatInformation.InstigatorTile->GetPlacementLocation());
	
	// CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	// CombatPrediction.CombatInformation.TargetUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);

	// CombatPrediction.CombatInformation.InstigatorUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	// CombatPrediction.CombatInformation.TargetUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);

	
	bCombatActive = true;
	if (OnCombatStart.IsBound()) { OnCombatStart.Broadcast(
		InCombatPrediction.CombatInformation.InstigatorUnit,
		InCombatPrediction.CombatInformation.TargetUnit
		); }
	
	SendCombatEventToNextUnit();
}

void UCombatWorldSubsystem::SendCombatEventToNextUnit()
{
	UE_LOG(Log_Combat, Error, TEXT("Sending CombatEvent to NextUnit"));
	
	ActiveUnit = CombatPrediction.CombatOrder[CombatOrderIndex];
	// ActiveUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	ActiveUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(this, &ThisClass::CacheGameplayAbility);
	ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::CheckGameplayAbility);
	
	// send gameplay event
	FGameplayEventData EventData;
	EventData.Instigator = ActiveUnit;
	EventData.Target = ActiveUnit == CombatPrediction.CombatInformation.InstigatorUnit ?
		CombatPrediction.CombatInformation.TargetUnit : CombatPrediction.CombatInformation.InstigatorUnit;
	
	UCombatEventWrapper* EventWrapper = NewObject<UCombatEventWrapper>();
	EventWrapper->InstigatorSnapShotAdvanced = ActiveUnit == CombatPrediction.CombatInformation.InstigatorUnit ?
		CombatPrediction.TargetSnapshotAdvanced : CombatPrediction.InstigatorSnapShotAdvanced;
	EventData.OptionalObject = EventWrapper;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, TAG_Event_Grid_Attack, EventData);
}

// void UCombatWorldSubsystem::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
void UCombatWorldSubsystem::OnGridUnitAbilityActivated(const FAbilityEndedData& AbilityEndedData)
{
	UGameplayAbility* InGameplayAbility = AbilityEndedData.AbilityThatEnded.Get();
	// if (!InGameplayAbility->AbilityTags.HasTagExact(TAG_Event_Grid_Attack))
	// {
	// 		UE_LOG(Log_Combat, Error, TEXT("In Gameplay Ability Does not have attack tag"));
	// 		return;
	// }
	// if (InGameplayAbility->IsActive())
	// {
	// 	UE_LOG(Log_Combat, Error, TEXT("In Gameplay Ability Is Active"));
	// 	return;
	// }
	if (!bCombatActive)
	{
		UE_LOG(Log_Combat, Error, TEXT("GridUnit Ability Activated outside active combat: %s"),
			*InGameplayAbility->GetAvatarActorFromActorInfo()->GetName());
	}
	else if (!CombatPrediction.CombatOrder.IsValidIndex(CombatOrderIndex))
	{
		UE_LOG(Log_Combat, Error, TEXT("CombatOrderIndex not valid for combat prediction: %s"), *CombatPrediction.GetString());
	}
	else if (InGameplayAbility->GetAvatarActorFromActorInfo() == CombatPrediction.CombatOrder[CombatOrderIndex])
	{
		float GameTime = GetWorld()->GetTimeSeconds();
		UE_LOG(Log_Combat, Warning, TEXT("Combat Order Index: %d"), CombatOrderIndex);
		UE_LOG(Log_Combat, Warning, TEXT("Actor Owning Ability: %s"), *InGameplayAbility->GetAvatarActorFromActorInfo()->GetName());
		UE_LOG(Log_Combat, Warning, TEXT("Game Time: %f"), GameTime);
		UE_LOG(Log_Combat, Warning, TEXT("Combat Prediction: %s, %s"),
			*CombatPrediction.CombatOrder[CombatOrderIndex]->GetName(), *CombatPrediction.Id.ToString());
		
		ActiveUnit = CombatPrediction.CombatOrder[CombatOrderIndex];
		ActiveUnit->GetAbilitySystemComponent()->OnAbilityEnded.RemoveAll(this);
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
			if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(CombatPrediction); }
		}
	}
}

void UCombatWorldSubsystem::CacheGameplayAbility(UGameplayAbility* InGameplayAbility)
{
	ActiveGameplayAbility = InGameplayAbility;
}

void UCombatWorldSubsystem::CheckGameplayAbility(UGameplayAbility* InGameplayAbility)
{
	if (ActiveGameplayAbility == InGameplayAbility)
	{
		float GameTime = GetWorld()->GetTimeSeconds();
		UE_LOG(Log_Combat, Warning, TEXT("Combat Order Index: %d"), CombatOrderIndex);
		UE_LOG(Log_Combat, Warning, TEXT("Actor Owning Ability: %s"), *InGameplayAbility->GetAvatarActorFromActorInfo()->GetName());
		UE_LOG(Log_Combat, Warning, TEXT("Game Time: %f"), GameTime);
		UE_LOG(Log_Combat, Warning, TEXT("Combat Prediction: %s, %s"),
			*CombatPrediction.CombatOrder[CombatOrderIndex]->GetName(), *CombatPrediction.Id.ToString());

		ActiveUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.RemoveAll(this);
		ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.RemoveAll(this);
		// ActiveUnit->GetAbilitySystemComponent()->OnAbilityEnded.RemoveAll(this);
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
			if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(CombatPrediction); }
		}
	}
}

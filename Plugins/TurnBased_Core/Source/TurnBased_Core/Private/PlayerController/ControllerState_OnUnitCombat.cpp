// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_OnUnitCombat.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TurnBased_Core_Tags.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"


UControllerState_OnUnitCombat::UControllerState_OnUnitCombat()
{
}

UControllerState_OnUnitCombat* UControllerState_OnUnitCombat::Create(
	AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit)
{
	UControllerState_OnUnitCombat* Object = NewObject<UControllerState_OnUnitCombat>();
	Object->InstigatorUnit = InInstigatorUnit;
	Object->TargetUnit = InTargetUnit;
	
	return Object;
}

void UControllerState_OnUnitCombat::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	UE_LOG(LogTemp, Error, TEXT("UControllerState_Combat"));
	
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	if (!IsValid(InstigatorUnit)) { PlayerController->PopState(); }
	UTurnWorldSubsystem* TurnWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (!TurnWorldSubsystem) { return; }
	if (!TurnWorldSubsystem->CanUnitTakeAction(InstigatorUnit)) { return; }

	InstigatorUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGridUnitAbilityActivated);
	
	FGameplayEventData EventData;
	EventData.Instigator = InstigatorUnit;
	EventData.Target = TargetUnit;

	// send gameplay event
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InstigatorUnit, TAG_Event_Grid_Attack, EventData);
}

void UControllerState_OnUnitCombat::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
{
	// only end when active unit finished ability...
	if (InGameplayAbility->GetAvatarActorFromActorInfo() == InstigatorUnit)
	{
		PlayerController->SetBaseState(UControllerState_Idle::Create());
	}	
}

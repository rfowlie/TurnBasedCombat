// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_OnUnitMove.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TurnBased_Core_Tags.h"
#include "GameplayAbilities/MoveAbilityPayload.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"

UControllerState_OnUnitMove::UControllerState_OnUnitMove()
{
}

UControllerState_OnUnitMove* UControllerState_OnUnitMove::Create(AGridUnit* InActiveUnit, AGridTileBase* InTargetTile)
{
	UControllerState_OnUnitMove* Object = NewObject<UControllerState_OnUnitMove>();
	Object->ActiveUnit = InActiveUnit;
	Object->TargetTile = InTargetTile;

	return Object;
}

void UControllerState_OnUnitMove::OnEnter(APlayerController* InPlayerController,
                                          const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	if (!IsValid(ActiveUnit) || !IsValid(TargetTile)) { PlayerController->PopState(); }
	UTurnWorldSubsystem* TurnWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (!TurnWorldSubsystem) { return; }
	if (!TurnWorldSubsystem->CanUnitTakeAction(ActiveUnit)) { return; }

	APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn());
	if (Pawn)
	{
		Pawn->SetFollowTarget(ActiveUnit);
	}
	
	GameplayAbilityDelegateHandle = ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(
			this, &ThisClass::OnGridUnitAbilityActivated);
	
	TArray<AGridTile*> Tiles;
	Tiles.Add(TargetTile);
	FGameplayEventData EventData;
	EventData.Instigator = ActiveUnit;	
	EventData.OptionalObject = UMoveAbilityPayload::Create(Tiles);

	// send gameplay event
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActiveUnit, TAG_Event_Grid_Move, EventData);
	
}

void UControllerState_OnUnitMove::OnExit()
{
	Super::OnExit();

	// unbind or else first event data will always fire
	ActiveUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.Remove(GameplayAbilityDelegateHandle);
}

void UControllerState_OnUnitMove::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
{
	// only end when active unit finished ability...
	if (InGameplayAbility->GetAvatarActorFromActorInfo() == ActiveUnit)
	{
		UTurnWorldSubsystem* TurnWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
		TurnWorldSubsystem->SetUnitTurnOver(ActiveUnit);
		TurnWorldSubsystem->CheckFactionTurnComplete(ActiveUnit->Execute_GetFaction(ActiveUnit));
		PlayerController->SetBaseState(UControllerState_Idle::Create());
	}	
}

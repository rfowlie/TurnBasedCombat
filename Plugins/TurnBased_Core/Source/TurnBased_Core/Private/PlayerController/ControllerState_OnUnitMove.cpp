﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_OnUnitMove.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TurnBased_Core_Tags.h"
#include "GameplayAbilities/MoveAbilityPayload.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"

UControllerState_OnUnitMove::UControllerState_OnUnitMove()
{
}

UControllerState_OnUnitMove* UControllerState_OnUnitMove::Create(AGridUnit* InActiveUnit, AGridTile* InTargetTile)
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

	ActiveUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(
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

	
}

void UControllerState_OnUnitMove::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
{
	// TODO: FOR NOW
	// save to assume that the GA is the move GA...
	PlayerController->SetBaseState(UControllerState_Idle::Create());
}

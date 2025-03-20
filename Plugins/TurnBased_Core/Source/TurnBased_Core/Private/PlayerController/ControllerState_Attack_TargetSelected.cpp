// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Attack_TargetSelected.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "TurnBased_Core_Tags.h"
#include "Grid/GridWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Attack_TileSelected.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"


UControllerState_Attack_TargetSelected::UControllerState_Attack_TargetSelected()
{
}

UControllerState_Attack_TargetSelected* UControllerState_Attack_TargetSelected::Create(
	AGridUnit* InActiveUnit, AGridUnit* InTargetUnit, const TArray<FGridMovement>& InGridMovements)
{
	auto Object = NewObject<UControllerState_Attack_TargetSelected>();
	Object->InstigatorUnit = InActiveUnit;
	Object->TargetUnit = InTargetUnit;
	Object->GridMovements = InGridMovements;
	return Object;
}

void UControllerState_Attack_TargetSelected::OnEnter(APlayerController* InPlayerController,
	const int32 InInputMappingContextPriority)
{
	UE_LOG(LogTemp, Error, TEXT("UControllerState_Attack_TargetSelected"));
	
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	UTurnWorldSubsystem* TurnSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
	if (!TurnSubsystem) { PlayerController->PopState(); }
	IsPlayerUnit = TurnSubsystem->CanUnitTakeAction(InstigatorUnit);
	
	// set state of instigator
	InstigatorUnit->SetState(TAG_TBCore_Grid_Tile_CanAttack);
	
	// get all tiles that target unit can be attacked from and set state
	if (UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		GridSubsystem->CalculateGridAttackTiles(AttackTileRangeMap, GridMovements, InstigatorUnit, TargetUnit);
		for (const auto Pair : AttackTileRangeMap)
		{
			Pair.Key->SetState(TAG_TBCore_Grid_Tile_CanAttack);
		}
	}
	
	// update cursor
	if (APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
	{
		Pawn->SetFollowTarget(TargetUnit);
	}
}

void UControllerState_Attack_TargetSelected::OnExit()
{
	Super::OnExit();

}

UInputMappingContext* UControllerState_Attack_TargetSelected::CreateInputMappingContext()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);
	
	InputAction_Select = NewObject<UInputAction>(this);
	InputAction_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(InputAction_Select, EKeys::LeftMouseButton);
	EIC->BindAction(InputAction_Select, ETriggerEvent::Started, this, &ThisClass::OnSelect);
	
	InputAction_Deselect = NewObject<UInputAction>(this);
	InputAction_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(InputAction_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(InputAction_Deselect, ETriggerEvent::Started, this, &ThisClass::OnDeselect);
	
	return NewInputMappingContext;	
}

void UControllerState_Attack_TargetSelected::OnSelect()
{
	// do not progress any further if not a player unit
	if (!IsPlayerUnit) { return; }

	// check if tile selected is in attack tiles map
	if (UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		if (AttackTileRangeMap.Contains(GridSubsystem->GridTileHovered))
		{
			PlayerController->PushState(UControllerState_Attack_TileSelected::Create(
				InstigatorUnit, TargetUnit, GridSubsystem->GridTileHovered, AttackTileRangeMap), false);
		}
	}
}

void UControllerState_Attack_TargetSelected::OnDeselect()
{
	PlayerController->PopState();
}

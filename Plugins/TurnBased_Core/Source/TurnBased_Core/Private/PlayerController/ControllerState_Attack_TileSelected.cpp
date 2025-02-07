// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Attack_TileSelected.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TurnBased_Core_Tags.h"
#include "PlayerController/ControllerState_Combat.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Tile/GridTile.h"
#include "UI/HUD_TurnBased.h"
#include "UI/UserWidget_TurnBased.h"
#include "Unit/GridUnit.h"


UControllerState_Attack_TileSelected::UControllerState_Attack_TileSelected()
{
}

UControllerState_Attack_TileSelected* UControllerState_Attack_TileSelected::Create(
	AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit, AGridTile* InTileSelected)
{
	UControllerState_Attack_TileSelected* Object = NewObject<UControllerState_Attack_TileSelected>();
	Object->InstigatorUnit = InInstigatorUnit;
	Object->TargetUnit = InTargetUnit;
	Object->TileSelected = InTileSelected;

	return Object;
}

void UControllerState_Attack_TileSelected::OnEnter(APlayerController* InPlayerController,
	const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// cache previous position
	InstigatorPreviousPosition = InstigatorUnit->GetTargetLocation();
	// move instigator to selected tile
	InstigatorUnit->SetActorLocation(TileSelected->GetPlacementLocation());
	// set state of instigator and selected tile
	InstigatorUnit->SetState(TAG_TBCore_Grid_Tile_Combat);
	TileSelected->SetState(TAG_TBCore_Grid_Tile_Combat);
	// pass units to combat UI, OR to combat subsystem (which will then take on passing all the relevant info?)
	if (AHUD_TurnBased* HUD = Cast<AHUD_TurnBased>(PlayerController->GetHUD()))
	{
		UUserWidget_TurnBased* Widget = HUD->ActivateCombatPredictionWidget(InstigatorUnit, TargetUnit);
		Widget->OnComplete.AddUniqueDynamic(this, &ThisClass::CombatInitiated);
	}	
}

void UControllerState_Attack_TileSelected::OnExit()
{
	Super::OnExit();

	// move instigator back to previous position
	// NOTE: reverting state should not be handled most of the time, should rely on
	// other controller states to set state in OnEnter!!
	// revert state of instigator and selected tile?
	
}

UInputMappingContext* UControllerState_Attack_TileSelected::CreateInputMappingContext()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);
	
	// InputAction_Select = NewObject<UInputAction>(this);
	// InputAction_Select->ValueType = EInputActionValueType::Boolean;
	// FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(InputAction_Select, EKeys::RightMouseButton);
	// EIC->BindAction(InputAction_Deselect, ETriggerEvent::Started, this, &ThisClass::OnSelect);
	
	InputAction_Deselect = NewObject<UInputAction>(this);
	InputAction_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(InputAction_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(InputAction_Deselect, ETriggerEvent::Started, this, &ThisClass::OnDeselect);
	
	return NewInputMappingContext;	
}

void UControllerState_Attack_TileSelected::OnDeselect()
{
	InstigatorUnit->SetActorLocation(InstigatorPreviousPosition);
	PlayerController->PopState();
}

void UControllerState_Attack_TileSelected::CombatInitiated()
{
	// recieve complete callback from combat widget indicating that combat is going to happen
	PlayerController->SetBaseState(UControllerState_Combat::Create());
}


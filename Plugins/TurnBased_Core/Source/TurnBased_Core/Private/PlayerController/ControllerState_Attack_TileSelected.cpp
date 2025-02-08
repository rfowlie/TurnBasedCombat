// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Attack_TileSelected.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TurnBased_Core_Tags.h"
#include "Grid/GridWorldSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerController/ControllerState_Combat.h"
#include "Tile/GridTile.h"
#include "UI/HUD_TurnBased.h"
#include "UI/UserWidget_TurnBased.h"
#include "Unit/GridUnit.h"


UControllerState_Attack_TileSelected::UControllerState_Attack_TileSelected()
{
}

UControllerState_Attack_TileSelected* UControllerState_Attack_TileSelected::Create(
	AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit, AGridTile* InTileSelected, const TMap<AGridTile*, int32>& InAttackTileRangeMap)
{
	UControllerState_Attack_TileSelected* Object = NewObject<UControllerState_Attack_TileSelected>();
	Object->InstigatorUnit = InInstigatorUnit;
	Object->TargetUnit = InTargetUnit;
	Object->TileSelected = InTileSelected;
	Object->AttackTileRangeMap = InAttackTileRangeMap;
	InAttackTileRangeMap.GetKeys(Object->AttackTileOrder);

	return Object;
}

void UControllerState_Attack_TileSelected::OnEnter(APlayerController* InPlayerController,
	const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);
	
	UE_LOG(LogTemp, Error, TEXT("UControllerState_Attack_TileSelected"));

	// cache previous position
	UGridWorldSubsystem* GridWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	if (!GridWorldSubsystem) { return; }
	InstigatorInitialTile = GridWorldSubsystem->GetGridTileOfUnit(InstigatorUnit);
	InstigatorInitialRotation = InstigatorUnit->GetActorRotation();
	
	// move instigator to selected tile
	InstigatorUnit->SetActorLocation(TileSelected->GetPlacementLocation());
	// set state of instigator and selected tile
	InstigatorUnit->SetState(TAG_TBCore_Grid_Tile_Combat);
	TileSelected->SetState(TAG_TBCore_Grid_Tile_Combat);
	// pass units to combat UI, OR to combat subsystem (which will then take on passing all the relevant info?)
	if (AHUD_TurnBased* HUD = Cast<AHUD_TurnBased>(PlayerController->GetHUD()))
	{
		UUserWidget_TurnBased* Widget = HUD->ActivateCombatPredictionWidget(InstigatorUnit, TargetUnit);
		if (Widget)
		{
			Widget->OnComplete.AddUniqueDynamic(this, &ThisClass::CombatInitiated);
		}
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
	
	InputAction_Select = NewObject<UInputAction>(this);
	InputAction_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(InputAction_Select, EKeys::LeftMouseButton);
	EIC->BindAction(InputAction_Select, ETriggerEvent::Started, this, &ThisClass::OnSelect);
	
	InputAction_Deselect = NewObject<UInputAction>(this);
	InputAction_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(InputAction_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(InputAction_Deselect, ETriggerEvent::Started, this, &ThisClass::OnDeselect);

	// TODO: let UI handle swapping weapons, as it has access to Instigator and Target
	// InputAction_CycleWeapon = NewObject<UInputAction>(this);
	// InputAction_CycleWeapon->ValueType = EInputActionValueType::Axis1D;
	// FEnhancedActionKeyMapping& Mapping_Cycle_Right = NewInputMappingContext->MapKey(InputAction_CycleWeapon, EKeys::E);
	// FEnhancedActionKeyMapping& Mapping_Cycle_Left = NewInputMappingContext->MapKey(InputAction_CycleWeapon, EKeys::Q);
	// Mapping_Cycle_Left.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	// EIC->BindAction(InputAction_CycleWeapon, ETriggerEvent::Started, this, &ThisClass::OnCycleTile);
	
	return NewInputMappingContext;	
}

void UControllerState_Attack_TileSelected::OnSelect()
{
	// check if tile selected is in attack tile
	if (UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		AGridTile* GridTileHovered = GridSubsystem->GridTileHovered;
		if (GridTileHovered && GridTileHovered != TileSelected && AttackTileRangeMap.Contains(GridTileHovered))
		{
			// undo previous, do new
			TileSelected->SetState(TAG_TBCore_Grid_Tile_CanAttack);
			TileSelected = GridTileHovered;
			TileSelected->SetState(TAG_TBCore_Grid_Tile_Combat);
			// update instigator position
			InstigatorUnit->SetActorLocation(TileSelected->GetPlacementLocation());
			
			// rotate to face
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				TileSelected->GetActorLocation(), TargetUnit->GetActorLocation());
			LookAtRotation.Pitch = 0.f;
			LookAtRotation.Roll = 0.f;
			InstigatorUnit->SetActorRotation(LookAtRotation);
		}
	}
}

void UControllerState_Attack_TileSelected::OnDeselect()
{
	InstigatorUnit->SetActorLocation(InstigatorInitialTile->GetPlacementLocation());
	InstigatorUnit->SetActorRotation(InstigatorInitialRotation);
	
	PlayerController->PopState();
}

void UControllerState_Attack_TileSelected::OnCycleTile(const FInputActionValue& Value)
{
	// int32 Index = AttackTileOrder.Find(TileSelected);
	// Index = (Index + FMath::TruncToInt(Value.GetMagnitude()) + AttackTileOrder.Num()) % AttackTileOrder.Num();
	//
	// // undo previous, do new
	// TileSelected->SetState(TAG_TBCore_Grid_Tile_CanAttack);
	// TileSelected = AttackTileOrder[Index];
	// TileSelected->SetState(TAG_TBCore_Grid_Tile_Combat);
	// // update instigator position
	// InstigatorUnit->SetActorLocation(TileSelected->GetPlacementLocation());
	//
	// // rotate to face
	// FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(InstigatorUnit->GetActorLocation(), TileSelected->GetActorLocation());
	// LookAtRotation.Pitch = 0.f;
	// LookAtRotation.Yaw = 0.f;
	// InstigatorUnit->SetActorRotation(LookAtRotation);

	// TODO: cycle weapon??? Let UI manage that...
}

void UControllerState_Attack_TileSelected::CombatInitiated()
{
	// receive complete callback from combat widget indicating that combat is going to happen
	PlayerController->SetBaseState(UControllerState_Combat::Create());
}


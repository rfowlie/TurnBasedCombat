// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Idle.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Grid/GridWorldSubsystem.h"
#include "PlayerController/ControllerState_UnitSelected.h"
#include "Turn/TurnWorldSubsystem.h"

struct FEnhancedActionKeyMapping;

UControllerState_Idle::UControllerState_Idle()
{
}

UControllerState_Idle* UControllerState_Idle::Create()
{
	UControllerState_Idle* Object = NewObject<UControllerState_Idle>();
	return Object;
}

void UControllerState_Idle::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// if (UGridWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	// {
	// 	Subsystem->OnGridUnitSelectedStart.AddUniqueDynamic(this, &ThisClass::OnUnitSelected);
	// }
}

void UControllerState_Idle::OnExit(const APlayerController* InPlayerController)
{
	Super::OnExit(InPlayerController);
	
	// if (UGridWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	// {
	// 	Subsystem->OnGridUnitSelectedStart.AddUniqueDynamic(this, &ThisClass::OnUnitSelected);
	// }
}

UInputMappingContext* UControllerState_Idle::CreateInputMappingContext(APlayerController* PlayerController)
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);

	InputAction_Select = NewObject<UInputAction>(this);
	InputAction_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(InputAction_Select, EKeys::LeftMouseButton);
	EIC->BindAction(InputAction_Select, ETriggerEvent::Started, this, &ThisClass::OnSelect);
	
	return NewInputMappingContext;
}

void UControllerState_Idle::OnSelect()
{
	if (UGridWorldSubsystem* GridSubsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		if (AGridUnit* SelectedUnit = GridSubsystem->GetGridUnitOnTile(GridSubsystem->GetGridTileHovered()))
		{
			if (UTurnWorldSubsystem* TurnSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
			{
				// TODO: need to decide how we want to handle selecting player/enemy unit
				OnChangedDelegate.Execute(UControllerState_UnitSelected::Create(
					SelectedUnit, TurnSubsystem->CanUnitTakeAction(SelectedUnit)));
				
			}
		}
	}
}

void UControllerState_Idle::OnUnitSelected(AGridUnit* InGridUnit) const
{
}

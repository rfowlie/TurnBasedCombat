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

UInputMappingContext* UControllerState_Idle::CreateInputMappingContext()
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
	if (UGridWorldSubsystem* GridSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		if (AGridUnit* SelectedUnit = GridSubsystem->GetGridUnitOnTile(GridSubsystem->GetGridTileHovered()))
		{
			if (UTurnWorldSubsystem* TurnSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
			{
				// OnChangedDelegate.Execute(UControllerState_UnitSelected::Create(
				// 	SelectedUnit, TurnSubsystem->CanUnitTakeAction(SelectedUnit)));

				PlayerController->PushState(UControllerState_UnitSelected::Create(
					SelectedUnit, TurnSubsystem->CanUnitTakeAction(SelectedUnit)), true);
				
			}
		}
	}
}

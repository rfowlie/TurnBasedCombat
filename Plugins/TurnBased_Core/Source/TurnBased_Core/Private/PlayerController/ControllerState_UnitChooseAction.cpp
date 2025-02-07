// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitChooseAction.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "PlayerController/ControllerState_Attack_TargetSelected.h"
#include "UI/UserWidget_ActionOptions.h"
#include "TurnBased_Core_Tags.h"
#include "UI/HUD_TurnBased.h"


UControllerState_UnitChooseAction::UControllerState_UnitChooseAction()
{
}

UControllerState_UnitChooseAction* UControllerState_UnitChooseAction::Create(AGridUnit* InActiveUnit)
{
	UControllerState_UnitChooseAction* Object = NewObject<UControllerState_UnitChooseAction>();
	return Object;
}

void UControllerState_UnitChooseAction::OnEnter(APlayerController* InPlayerController,
	const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// display action UI
	if (!Widget_ActionOptions)
	{
		// try to get widget through HUD
		if (AHUD_TurnBased* HUD = Cast<AHUD_TurnBased>(PlayerController->GetHUD()))
		{
			// this function should let all the setup and placement for the HUD to handle
			// if this is a blocking widget then it should stop cursor movement...

			// TODO: this is slightly problematic, passing in the actions here...
			// determine which actions this unit can take? do this here in the HUD?
			TArray<FGameplayTag> ActionTags;
			ActionTags.Add(TAG_TBCore_Action_Attack);
			ActionTags.Add(TAG_TBCore_Action_Skill);
			ActionTags.Add(TAG_TBCore_Action_Item);
			Widget_ActionOptions = HUD->ActivateActionOptionsWidget(ActiveUnit, ActionTags);
			// Widget_ActionOptions->OnActionSelected.AddUniqueDynamic(this, &ThisClass::OnActionSelected);
		}
		
		// Widget_ActionOptions = Cast<UWidget_ActionOptions>(CreateWidget(PlayerController, PlayerController->ActionWidgetClass));
		if (!Widget_ActionOptions) { UE_LOG(LogTemp, Error, TEXT("Widget ActionOptions is null")); }
		else
		{
			Widget_ActionOptions->OnActionSelected.AddUniqueDynamic(this, &ThisClass::OnActionSelected);
		}		
	}
}

void UControllerState_UnitChooseAction::OnExit()
{
	Super::OnExit();

	// have HUD fire callback for other listeners
	if (AHUD_TurnBased* HUD = Cast<AHUD_TurnBased>(PlayerController->GetHUD()))
	{
		HUD->RemoveActionOptionsWidget();
	}
}

void UControllerState_UnitChooseAction::OnActionSelected(FGameplayTag ActionTag)
{
	if (ActionTag.MatchesTagExact(TAG_TBCore_Action_Attack))
	{
		
	}	
}

UInputMappingContext* UControllerState_UnitChooseAction::CreateInputMappingContext()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);
	
	InputAction_Deselect = NewObject<UInputAction>(this);
	InputAction_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(InputAction_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(InputAction_Deselect, ETriggerEvent::Started, this, &ThisClass::OnDeselect);
	
	return NewInputMappingContext;
}

void UControllerState_UnitChooseAction::OnDeselect()
{
	// remove self from callback
	if (Widget_ActionOptions){ Widget_ActionOptions->OnActionSelected.RemoveDynamic(this, &ThisClass::OnActionSelected); }	
	PlayerController->PopState();
}

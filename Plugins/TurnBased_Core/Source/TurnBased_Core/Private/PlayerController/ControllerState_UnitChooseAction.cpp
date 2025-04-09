// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitChooseAction.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "PlayerController/ControllerState_Attack_TargetSelected.h"
#include "UI/UserWidget_ActionOptions.h"
#include "TurnBased_Core_Tags.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_OnUnitMove.h"
#include "UI/HUD_TurnBased.h"


UControllerState_UnitChooseAction::UControllerState_UnitChooseAction()
{
}

UControllerState_UnitChooseAction* UControllerState_UnitChooseAction::Create(
	AGridUnit* InActiveUnit, AGridTileBase* InTargetTile)
{
	UControllerState_UnitChooseAction* Object = NewObject<UControllerState_UnitChooseAction>();
	Object->ActiveUnit = InActiveUnit;
	Object->TargetTile = InTargetTile;
	
	return Object;
}

void UControllerState_UnitChooseAction::OnEnter(APlayerController* InPlayerController,
	const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// try to get widget through HUD
	if (AHUD_TurnBased* HUD = Cast<AHUD_TurnBased>(PlayerController->GetHUD()))
	{
		// this function should let all the setup and placement for the HUD to handle
		// if this is a blocking widget then it should stop cursor movement...

		// TODO: this is slightly problematic, passing in the actions here...
		// determine which actions this unit can take? do this here in the HUD?
		TArray<FGameplayTag> ActionTags;
		ActionTags.Add(TAG_TBCore_Action_Skill);
		ActionTags.Add(TAG_TBCore_Action_Item);
		ActionTags.Add(TAG_TBCore_Action_Wait);
		Widget_ActionOptions = HUD->ActivateActionOptionsWidget(ActiveUnit, ActionTags);
		// Widget_ActionOptions->OnActionSelected.AddUniqueDynamic(this, &ThisClass::OnActionSelected);
		// Widget_ActionOptions = Cast<UWidget_ActionOptions>(CreateWidget(PlayerController, PlayerController->ActionWidgetClass));
		if (!Widget_ActionOptions)
		{
			UE_LOG(LogTemp, Error, TEXT("Widget ActionOptions is null"));
		}
		else
		{
			Widget_ActionOptions->SetVisibility(ESlateVisibility::Visible);
			Widget_ActionOptions->OnActionSelected.AddUniqueDynamic(this, &ThisClass::OnActionSelected);
		}

		// update cursor
		if (APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
		{
			Pawn->SetCursorCanTick(false);
		}
	}
}

void UControllerState_UnitChooseAction::OnExit()
{
	Super::OnExit();
	
	if (Widget_ActionOptions)
	{
		Widget_ActionOptions->OnActionSelected.RemoveDynamic(this, &ThisClass::OnActionSelected);

		// TODO: this is a bit hacky, should call up to HUD
		Widget_ActionOptions->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// update cursor
	if (APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
	{
		Pawn->SetCursorCanTick(true);
	}
}

void UControllerState_UnitChooseAction::OnActionSelected(FGameplayTag ActionTag)
{
	if (ActionTag.MatchesTagExact(TAG_TBCore_Action_Wait))
	{
		PlayerController->PushState(UControllerState_OnUnitMove::Create(ActiveUnit, TargetTile), true);
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
	// if (Widget_ActionOptions)
	// {
	// 	Widget_ActionOptions->OnActionSelected.RemoveDynamic(this, &ThisClass::OnActionSelected);
	// }	
	PlayerController->PopState();
}

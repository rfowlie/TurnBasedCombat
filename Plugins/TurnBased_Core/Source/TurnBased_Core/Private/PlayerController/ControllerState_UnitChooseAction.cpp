// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_UnitChooseAction.h"
#include "PlayerController/ControllerState_UnitAttack.h"
#include "PlayerController/UI/Widget_ActionOptions.h"
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

			// determine which actions this unit can take? do this here in the HUD?
			TArray<FGameplayTag> ActionTags = GetActionTags();
			Widget_ActionOptions = HUD->ActivateAction(ActiveUnit,ActionTags);
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

	Widget_ActionOptions->OnActionSelected.RemoveDynamic(this, &ThisClass:: OnActionSelected);

	// hide action UI??? Or let this happen elsewhere like HUD or the widget itself???
}

void UControllerState_UnitChooseAction::OnActionSelected(FGameplayTag ActionTag)
{
	if (ActionTag.MatchesTagExact(TAG_TBCore_Action_Attack))
	{
		PlayerController->PushState(UControllerState_UnitAttack::Create(ActiveUnit), true);
	}	
}

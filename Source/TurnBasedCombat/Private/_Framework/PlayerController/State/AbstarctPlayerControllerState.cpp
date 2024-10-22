// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedInputSubsystems.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/State/AbstractPlayerControllerState.h"


UAbstractPlayerControllerState::UAbstractPlayerControllerState()
{
}

void UAbstractPlayerControllerState::CreateInput()
{
}

void UAbstractPlayerControllerState::EnableInput()
{
}

void UAbstractPlayerControllerState::DisableInput()
{
}

// void UAbstractPlayerControllerState::OnSelect()
// {
// }
//
// void UAbstractPlayerControllerState::OnDeselect()
// {
// }
//
// void UAbstractPlayerControllerState::OnCycleUnit()
// {
// }

void UAbstractPlayerControllerState::Enter(APlayerController* PlayerController,	const int32 InputMappingContextPriority)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter - base implementation"));
	
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("State::Enter - Player Controller Null"));
		return;
	}
	
	// guarantees that input mapping context has a chance to initialize, or reinitialize if GC
	if (!InputMappingContext) { InputMappingContext = SetupInputMappingContext(PlayerController); }	
	if (InputMappingContext)
	{
		UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		check(EILP)
		EILP->AddMappingContext(InputMappingContext, InputMappingContextPriority);
	}
	
	OnEnter();
}

void UAbstractPlayerControllerState::Exit(const APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit - base implementation"));
	
	// remove input mapping context
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	checkf(EILP, TEXT("UEnhancedInputLocalPlayerSubsystem null, player controller local player might have issue"))
	EILP->RemoveMappingContext(InputMappingContext);
	OnExit();
}

UInputMappingContext* UAbstractPlayerControllerState::SetupInputMappingContext(APlayerController* PlayerController)
{
	return nullptr;
}

void UAbstractPlayerControllerState::OnEnter()
{
}

void UAbstractPlayerControllerState::OnExit()
{
}



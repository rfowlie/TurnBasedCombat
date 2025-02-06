// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Abstract.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"


FGameplayTag UControllerState_Abstract::GetStateTag() const
{
	return FGameplayTag();
}

void UControllerState_Abstract::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	// UE_LOG(LogTemp, Warning, TEXT("Controller State Enter - Abstract"));
	
	if (!InPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller State Enter: player controller null"));
		return;
	}

	PlayerController = Cast<APlayerController_TurnBased>(InPlayerController);

	// guarantees that input mapping context has a chance to initialize, or reinitialize if GC
	InputMappingContextPriority = InInputMappingContextPriority;
	if (!InputMappingContext) { InputMappingContext = CreateInputMappingContext(); }
	if (InputMappingContext)
	{
		OnEnable();
	}
}

void UControllerState_Abstract::OnExit()
{	
	OnDisable();
}

void UControllerState_Abstract::OnEnable()
{
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	check(EILP)
	EILP->AddMappingContext(InputMappingContext, InputMappingContextPriority);
}

void UControllerState_Abstract::OnDisable()
{
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	checkf(EILP, TEXT("UEnhancedInputLocalPlayerSubsystem null, player controller local player might have issue"))
	EILP->RemoveMappingContext(InputMappingContext);
}

UInputMappingContext* UControllerState_Abstract::CreateInputMappingContext()
{
	return nullptr;
}

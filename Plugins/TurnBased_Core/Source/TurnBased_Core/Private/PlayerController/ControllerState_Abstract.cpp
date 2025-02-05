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
	
	// guarantees that input mapping context has a chance to initialize, or reinitialize if GC
	InputMappingContextPriority = InInputMappingContextPriority;
	if (!InputMappingContext) { InputMappingContext = CreateInputMappingContext(InPlayerController); }	
	if (InputMappingContext)
	{
		OnEnable(InPlayerController);
	}
}

void UControllerState_Abstract::OnExit(const APlayerController* InPlayerController)
{	
	OnDisable(InPlayerController);
}

void UControllerState_Abstract::OnEnable(const APlayerController* PlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	check(EILP)
	EILP->AddMappingContext(InputMappingContext, InputMappingContextPriority);
}

void UControllerState_Abstract::OnDisable(const APlayerController* InPlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InPlayerController->GetLocalPlayer());
	checkf(EILP, TEXT("UEnhancedInputLocalPlayerSubsystem null, player controller local player might have issue"))
	EILP->RemoveMappingContext(InputMappingContext);
}

UInputMappingContext* UControllerState_Abstract::CreateInputMappingContext(APlayerController* PlayerController)
{
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_Wait.h"
#include "Combat/CombatWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"


UControllerState_Wait::UControllerState_Wait()
{
}

UControllerState_Wait* UControllerState_Wait::Create()
{
	UControllerState_Wait* Object = NewObject<UControllerState_Wait>();
	return Object;
}

void UControllerState_Wait::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	// stop any cursor movement
	PlayerController->ShowTileCursor(false);
	if (APawn_FollowCursor* FollowPawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
	{
		FollowPawn->SetCursorCanTick(false);
	}	

	// bind to combats occuring to update camera
	UCombatWorldSubsystem* CombatWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UCombatWorldSubsystem>();
	CombatWorldSubsystem->OnCombatStart.AddUniqueDynamic(this, &ThisClass::OnCombatStart);
}

void UControllerState_Wait::OnCombatStart(AGridUnit* Instigator, AGridUnit* Target)
{
	if (!IsValid(Instigator) || !IsValid(Target)) { return; }
	
	if (APawn_FollowCursor* FollowPawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
	{
		FVector MoveToLocation = (Instigator->GetActorLocation() + Target->GetActorLocation()) / 2;
		FollowPawn->SetMoveToLocation(MoveToLocation);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_OnUnitCombat.h"
#include "Combat/CombatWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"


UControllerState_OnUnitCombat::UControllerState_OnUnitCombat()
{
}

UControllerState_OnUnitCombat* UControllerState_OnUnitCombat::Create(const FCombatPrediction& InCombatPrediction)
{
	UControllerState_OnUnitCombat* Object = NewObject<UControllerState_OnUnitCombat>();
	Object->CombatPrediction = InCombatPrediction;
	
	return Object;
}

void UControllerState_OnUnitCombat::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	UE_LOG(LogTemp, Error, TEXT("UControllerState_Combat"));
	
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	if (APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn()))
	{
		Pawn->SetFollowTarget(CombatPrediction.CombatInformation.TargetUnit);
	}
	
	UCombatWorldSubsystem* CombatWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UCombatWorldSubsystem>();
	CombatWorldSubsystem->OnCombatEnd.AddUniqueDynamic(this, &ThisClass::OnCombatEnd);
	CombatWorldSubsystem->InitiateCombat(CombatPrediction);	
}

void UControllerState_OnUnitCombat::OnExit()
{
	Super::OnExit();

	UCombatWorldSubsystem* CombatWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UCombatWorldSubsystem>();
	CombatWorldSubsystem->OnCombatEnd.RemoveAll(this);
}

void UControllerState_OnUnitCombat::OnCombatEnd(const FCombatPrediction& InCombatPrediction)
{
	if (CombatPrediction.Id == InCombatPrediction.Id)
	{
		// // Update unit that attacked
		// UTurnWorldSubsystem* TurnWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
		// TurnWorldSubsystem->SetUnitTurnOver(CombatPrediction.CombatInformation.InstigatorUnit);
		
		// Update unit that attacked
		UTurnWorldSubsystem* TurnWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UTurnWorldSubsystem>();
		TurnWorldSubsystem->SetUnitTurnOver(CombatPrediction.CombatInformation.InstigatorUnit);
		TurnWorldSubsystem->CheckFactionTurnComplete(
			CombatPrediction.CombatInformation.InstigatorUnit->Execute_GetFaction(CombatPrediction.CombatInformation.InstigatorUnit));
		
		// reset prediction so it does not fire again...
		CombatPrediction = FCombatPrediction();
		
		// reset controller state
		PlayerController->SetBaseState(UControllerState_Idle::Create());
	}
	else
	{
		UE_LOG(Log_Combat, Error, TEXT("UControllerState_OnUnitCombat::OnCombatEnd - received different combat prediction"));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_OnUnitCombat.h"
#include "Combat/CombatWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Unit/GridUnit.h"


UControllerState_OnUnitCombat::UControllerState_OnUnitCombat()
{
}

UControllerState_OnUnitCombat* UControllerState_OnUnitCombat::Create(
	AGridUnit* InInstigatorUnit, AGridUnit* InTargetUnit)
{
	UControllerState_OnUnitCombat* Object = NewObject<UControllerState_OnUnitCombat>();
	Object->InstigatorUnit = InInstigatorUnit;
	Object->TargetUnit = InTargetUnit;
	
	return Object;
}

void UControllerState_OnUnitCombat::OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority)
{
	UE_LOG(LogTemp, Error, TEXT("UControllerState_Combat"));
	
	Super::OnEnter(InPlayerController, InInputMappingContextPriority);

	if (UCombatWorldSubsystem* CombatSubsystem = PlayerController->GetWorld()->GetSubsystem<UCombatWorldSubsystem>())
	{
		CombatSubsystem->OnCombatEnd.AddUniqueDynamic(this, &ThisClass::OnCombatEnd);
		CombatSubsystem->InitiateCombat(InstigatorUnit, TargetUnit);
		APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn());
		if (Pawn)
		{
			Pawn->SetFollowTarget(TargetUnit);
		}
	}
}

void UControllerState_OnUnitCombat::OnCombatEnd(const AGridUnit* Instigator, const AGridUnit* Target)
{
	PlayerController->SetBaseState(UControllerState_Idle::Create());
}

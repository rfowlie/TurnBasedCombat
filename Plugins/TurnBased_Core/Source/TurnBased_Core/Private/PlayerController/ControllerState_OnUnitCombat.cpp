// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ControllerState_OnUnitCombat.h"

#include "Combat/CombatCalculator_Basic.h"
#include "Combat/CombatWorldSubsystem.h"
#include "GameMode/GameMode_TurnBased_Combat.h"
#include "Grid/GridHelper.h"
#include "Grid/GridWorldSubsystem.h"
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

	UCombatWorldSubsystem* CombatSubsystem = PlayerController->GetWorld()->GetSubsystem<UCombatWorldSubsystem>();	
	CombatSubsystem->OnCombatEnd.AddUniqueDynamic(this, &ThisClass::OnCombatEnd);
	UGridWorldSubsystem* GridWorldSubsystem = PlayerController->GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(PlayerController->GetWorld()->GetAuthGameMode());

	FCombatPrediction OutCombatPrediction;
	FCombatInformation CombatInformation;
	CombatInformation.InstigatorUnit = InstigatorUnit;
	CombatInformation.InstigatorTile = GridWorldSubsystem->GetGridTileOfUnit(InstigatorUnit);
	CombatInformation.InstigatorWeapon = InstigatorUnit->GetEquippedWeaponName();
	CombatInformation.TargetUnit = TargetUnit;
	CombatInformation.TargetTile = GridWorldSubsystem->GetGridTileOfUnit(TargetUnit);
	CombatInformation.TargetWeapon = TargetUnit->GetEquippedWeaponName();
	GameMode->GetCombatCalculator()->GetCombatPrediction(OutCombatPrediction, CombatInformation);

	CombatSubsystem->InitiateCombat(OutCombatPrediction);
	APawn_FollowCursor* Pawn = Cast<APawn_FollowCursor>(PlayerController->GetPawn());
	if (Pawn)
	{
		Pawn->SetFollowTarget(TargetUnit);
	}
}

void UControllerState_OnUnitCombat::OnCombatEnd(const FCombatPrediction& InCombatPrediction)
{
	PlayerController->SetBaseState(UControllerState_Idle::Create());
}

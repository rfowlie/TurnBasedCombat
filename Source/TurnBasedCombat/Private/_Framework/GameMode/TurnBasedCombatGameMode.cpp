// Fill out your copyright notice in the Description page of Project Settings.



#include "TurnBasedCombat/Public/_Framework/GameMode/TurnBasedCombatGameMode.h"
#include "TurnBasedCombat/Public/EventSystem/EventSystem.h"
#include "TurnBasedCombat/Public/Grid/GridManager.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/TurnBasedCombatPlayerController.h"


ATurnBasedCombatGameMode::ATurnBasedCombatGameMode()
{
	PlayerControllerClass = ATurnBasedCombatPlayerController::StaticClass();
	
	EventSystem = NewObject<UEventSystem>(this);
	GridManager = NewObject<UGridManager>(this);
}

void ATurnBasedCombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

const UEventSystem* ATurnBasedCombatGameMode::GetEventSystem() const
{
	return EventSystem;
}

// const UGridManager* ATurnBasedCombatGameMode::GetGridManager() const
// {
// 	return GridManager;
// }

void ATurnBasedCombatGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (ATurnBasedCombatPlayerController* PlayerController = Cast<ATurnBasedCombatPlayerController>(NewPlayer))
	{
		PlayerController->Initialize(GridManager);
	}
}

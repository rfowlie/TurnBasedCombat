// Fill out your copyright notice in the Description page of Project Settings.



#include "TurnBasedCombat/Public/_Framework/GameMode/TurnBasedCombatGameMode.h"
#include "TurnBasedCombat/Public/EventSystem/EventSystem.h"
#include "TurnBasedCombat/Public/Grid/GridManager.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/TurnBasedCombatPlayerController.h"


ATurnBasedCombatGameMode::ATurnBasedCombatGameMode()
{
	PlayerControllerClass = ATurnBasedCombatPlayerController::StaticClass();
	
	// EventSystem = NewObject<UEventSystem>(this);
	EventSystem = CreateDefaultSubobject<UEventSystem>(TEXT("EventSystem"));
	// GridManager = NewObject<UGridManager>(this);
	GridManager = CreateDefaultSubobject<UGridManager>(TEXT("GridManager"));
}

void ATurnBasedCombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

UEventSystem* ATurnBasedCombatGameMode::GetEventSystem()
{
	return EventSystem;
}

UGridManager* ATurnBasedCombatGameMode::GetGridManager()
{
	return GridManager;
}

void ATurnBasedCombatGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (ATurnBasedCombatPlayerController* PlayerController = Cast<ATurnBasedCombatPlayerController>(NewPlayer))
	{
		PlayerController->Initialize(GridManager);
	}
}

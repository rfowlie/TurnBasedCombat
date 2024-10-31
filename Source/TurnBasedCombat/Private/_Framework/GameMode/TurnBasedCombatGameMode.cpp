// Fill out your copyright notice in the Description page of Project Settings.



#include "TurnBasedCombat/Public/_Framework/GameMode/TurnBasedCombatGameMode.h"

#include "Engine/StaticMeshActor.h"
#include "Grid/Manager/TurnManager.h"
#include "Grid/Tile/GridTile.h"
#include "TurnBasedCombat/Public/EventSystem/EventSystem.h"
#include "TurnBasedCombat/Public/Grid/Manager/GridManager.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/TurnBasedCombatPlayerController.h"


ATurnBasedCombatGameMode::ATurnBasedCombatGameMode()
{
	PlayerControllerClass = ATurnBasedCombatPlayerController::StaticClass();
	
	EventSystem = CreateDefaultSubobject<UEventSystem>(TEXT("EventSystem"));
	
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
	TurnManager->OnFactionWin.BindUObject(this, &ThisClass::OnFactionWin);
	
	GridManager = CreateDefaultSubobject<UGridManager>(TEXT("GridManager"));
	GridManager->Initialize(TurnManager);
	// game mode will act as facade and hold blueprint assignable delegate for OnEventStart/End
	// when game manger fires on event start, the game mode will do some stuff then broadcast to the rest of game
	GridManager->OnGridEventStart.AddLambda([this]()
	{
		// other stuff to do...
		UE_LOG(LogTemp, Warning, TEXT("Game Mode: OnGridEventStart"));
		if (OnGridEventStart.IsBound()) { OnGridEventStart.Broadcast(); }
	});
	GridManager->OnGridEventEnd.AddLambda([this]()
	{
		// other stuff to do...
		UE_LOG(LogTemp, Warning, TEXT("Game Mode: OnGridEventEnd"));
		if (OnGridEventEnd.IsBound()) { OnGridEventEnd.Broadcast(); }
	});
}

void ATurnBasedCombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	// create cursor
	const auto SpawnTransform = FTransform::Identity;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = TEXT("FUCKING CURSOR");
	SpawnParameters.Owner = this;
	Cursor = GetWorld()->SpawnActor(AStaticMeshActor::StaticClass(), &SpawnTransform, SpawnParameters);
	UStaticMeshComponent* StaticMeshComponent = Cast<AStaticMeshActor>(Cursor)->GetStaticMeshComponent();
	StaticMeshComponent->SetStaticMesh(CursorMesh);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	GridManager->OnGridTileHovered.AddDynamic(this, &ThisClass::UpdateCursor);

	// TODO: HACKYYY
	// for now wait a few seconds then fire onstart
	UE_LOG(LogTemp, Error, TEXT("On Combat Start Prep"));
	GetWorldTimerManager().SetTimer(
		Handle,
		[this]()
		{
			UE_LOG(LogTemp, Error, TEXT("On Combat Start Fire"));
			TurnManager->SetNextFaction();
			if (OnCombatStart.IsBound()) { OnCombatStart.Broadcast(); }
		},
		3,
		false);	
}

UEventSystem* ATurnBasedCombatGameMode::GetEventSystem()
{
	return EventSystem;
}

// UGridManager* ATurnBasedCombatGameMode::GetGridManager()
// {
// 	return GridManager;
// }

void ATurnBasedCombatGameMode::RegisterGridTile(AGridTile* GridTile)
{
	GridManager->RegisterGridTile(GridTile);
}

void ATurnBasedCombatGameMode::RegisterGridUnit(AGridUnit* GridUnit)
{
	GridManager->RegisterGridUnit(GridUnit);
}

void ATurnBasedCombatGameMode::UpdateCursor(const AGridTile* GridTile)
{
	Cursor->SetActorLocation(GridTile->GetActorLocation() + Cursor_ExtraHeight);
}

void ATurnBasedCombatGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (ATurnBasedCombatPlayerController* PlayerController = Cast<ATurnBasedCombatPlayerController>(NewPlayer))
	{
		PlayerController->Initialize(GridManager);
	}
}

void ATurnBasedCombatGameMode::OnFactionWin(FName Faction)
{
	// TODO
	UE_LOG(LogTemp, Warning, TEXT("WIN: %s"), *Faction.ToString());
}

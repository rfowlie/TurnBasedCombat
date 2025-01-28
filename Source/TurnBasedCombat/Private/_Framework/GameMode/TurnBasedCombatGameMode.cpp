// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnBasedCombat/Public/_Framework/GameMode/TurnBasedCombatGameMode.h"
#include "Engine/StaticMeshActor.h"
#include "Grid/Manager/GridRules.h"
#include "Grid/Manager/TurnManager.h"
#include "Grid/Tile/GridTile.h"
#include "TurnBasedCombat/Public/EventSystem/EventSystem.h"
#include "TurnBasedCombat/Public/Grid/Manager/GridManager.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/TurnBasedCombatPlayerController.h"
#include "_Framework/TBC_InfoWorldSubsystem.h"
#include "_Framework/GameMode/WinCondition_Abstract.h"
#include "_Framework/HUD/TurnBasedCombatHUD.h"


ATurnBasedCombatGameMode::ATurnBasedCombatGameMode()
{
	// setup default classes???
	PlayerControllerClass = ATurnBasedCombatPlayerController::StaticClass();
	HUDClass = ATurnBasedCombatHUD::StaticClass();

	// create internal classes
	EventSystem = CreateDefaultSubobject<UEventSystem>(TEXT("EventSystem"));
	GridRules = CreateDefaultSubobject<UGridRules>(TEXT("GridRules"));
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));	
	GridManager = CreateDefaultSubobject<UGridManager>(TEXT("GridManager"));
	GridManager->Initialize(GridRules, TurnManager);

	// game mode will act as facade and hold blueprint assignable delegate for OnEventStart/End
	// when game manger fires on event start, the game mode will do some stuff then broadcast to the rest of game
	GridManager->OnGridEventStart.AddLambda([this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Mode: OnGridEventStart"));
		if (OnGridEventStart.IsBound()) { OnGridEventStart.Broadcast(); }
	});
	GridManager->OnGridEventEnd.AddLambda([this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Mode: OnGridEventEnd"));
		if (OnGridEventEnd.IsBound()) { OnGridEventEnd.Broadcast(); }
	});
	GridManager->OnGridTileHovered.AddLambda([this](const AGridTile* Tile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Mode: OnGridTileHovered"));
		if (OnGridTileHovered.IsBound()) { OnGridTileHovered.Broadcast(Tile); }
		UpdateCursor(Tile);
	});
	GridManager->OnGridUnitHovered.AddLambda([this](const AGridUnit* Unit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Mode: OnGridUnitHovered"));
		if (OnGridUnitHovered.IsBound()) { OnGridUnitHovered.Broadcast(Unit); }
	});

	// Win condition setup
	WinCondition = CreateDefaultSubobject<UWinCondition_Abstract>(TEXT("WinCondition"));
	
}

void ATurnBasedCombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	// combat calculator setup
	if (CombatCalculatorClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Mode: CombatCalculator class is null"));
	}
	else
	{
		CombatCalculator = NewObject<UCombatCalculator>(this, CombatCalculatorClass);
		CombatCalculator->SetGridManager(GridManager);
	}	
	
	if (UTBC_InfoWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UTBC_InfoWorldSubsystem>())
	{
		Subsystem->SetCombatCalculator(CombatCalculator);
	}

	// Win condition setup
	if (WinCondition)
	{
		TurnManager->OnFactionDefeated.BindUObject(WinCondition, &UWinCondition_Abstract::CheckFactionDefeated);
		WinCondition->OnWinCondition.BindUObject(this, &ThisClass::OnWinConditionReceived);
	}
	
	// create cursor
	const auto SpawnTransform = FTransform::Identity;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = TEXT("FUCKING CURSOR");
	SpawnParameters.Owner = this;
	Cursor = GetWorld()->SpawnActor(AStaticMeshActor::StaticClass(), &SpawnTransform, SpawnParameters);
	UStaticMeshComponent* StaticMeshComponent = Cast<AStaticMeshActor>(Cursor)->GetStaticMeshComponent();
	StaticMeshComponent->SetStaticMesh(CursorMesh);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetAffectDistanceFieldLighting(false);
	StaticMeshComponent->SetAffectDynamicIndirectLighting(false);

	// TODO: HACKYYY
	// for now wait a few seconds then fire onstart
	UE_LOG(LogTemp, Error, TEXT("On Combat Start Prep"));
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(
		Handle,
		[this]()
		{
			UE_LOG(LogTemp, Error, TEXT("On Combat Start Fire"));
			TurnManager->SetNextFaction();
			if (OnCombatEnable.IsBound()) { OnCombatEnable.Broadcast(); }
		},
		3,
		false);	
}

UEventSystem* ATurnBasedCombatGameMode::GetEventSystem()
{
	return EventSystem;
}

void ATurnBasedCombatGameMode::RegisterGridTile(AGridTile* GridTile)
{
	GridManager->RegisterGridTile(GridTile);
}

void ATurnBasedCombatGameMode::RegisterGridUnit(AGridUnit* GridUnit)
{
	GridManager->RegisterGridUnit(GridUnit);

	// Win condition setup
	// should be aware of when units die to trigger win conditions
	// for example, if the main character in FE dies you lose the level automatically
	if (WinCondition)
	{
		GridUnit->OnDefeat.AddDynamic(WinCondition, &UWinCondition_Abstract::CheckGridUnitDefeated);
	}	
}

void ATurnBasedCombatGameMode::UpdateCursor(const AGridTile* GridTile)
{
	Cursor->SetActorLocation(GridTile->GetActorLocation() + Cursor_ExtraHeight);
}

void ATurnBasedCombatGameMode::OnWinConditionReceived(EWinConditionType InWinCondition)
{
	// stop internal things like player controller...
	if (OnCombatDisable.IsBound()) { OnCombatDisable.Broadcast(); }
	// notify everything that condition met
	if (OnCombatOver.IsBound()) { OnCombatOver.Broadcast(InWinCondition); }
	
	// TODO: what other things will need to happen internally???
	if (InWinCondition == Win)
	{
		UE_LOG(LogTemp, Error, TEXT("Win Condition: Win"));
	}
	else if (InWinCondition == Defeat)
	{
		UE_LOG(LogTemp, Error, TEXT("Win Condition: Defeat"));
	}	
}

void ATurnBasedCombatGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (ATurnBasedCombatPlayerController* PlayerController = Cast<ATurnBasedCombatPlayerController>(NewPlayer))
	{
		PlayerController->Initialize(GridManager);
	}
}

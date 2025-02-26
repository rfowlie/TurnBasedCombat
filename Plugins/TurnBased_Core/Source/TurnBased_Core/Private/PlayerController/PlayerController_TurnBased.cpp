// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_TurnBased.h"
#include "Combat/CombatWorldSubsystem.h"
#include "Engine/StaticMeshActor.h"
#include "Grid/GridWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"


void APlayerController_TurnBased::BeginPlay()
{
	Super::BeginPlay();

	// create cursor
	CreateCursor();
	
	// bind cursor update to grid tile hovered
	if (UGridWorldSubsystem* GridSubsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		GridSubsystem->OnGridTileHoveredStart.AddUniqueDynamic(this, &ThisClass::UpdateCursor);
	}
	
	if (UCombatWorldSubsystem* CombatSubsystem = GetWorld()->GetSubsystem<UCombatWorldSubsystem>())
	{
		// CombatSubsystem->OnCombatStart.AddUniqueDynamic(this, &ThisClass::OnCombatStart);
		// CombatSubsystem->OnCombatEnd.AddUniqueDynamic(this, &ThisClass::OnCombatEnd);
	}
	if (UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
	{
		TurnWorldSubsystem->OnFactionStart.AddUniqueDynamic(this, &ThisClass::OnFactionStart);
	}

	// create initial state idle
	PushState(NewObject<UControllerState_Idle>(), false);
}

APlayerController_TurnBased::APlayerController_TurnBased()
{
	// enable mouse interaction
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
}

void APlayerController_TurnBased::CreateCursor()
{
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
	// TODO: better way to do this???
	StaticMeshComponent->SetWorldScale3D(FVector(2.f, 2.f, 1.f));
}

void APlayerController_TurnBased::UpdateCursor(AGridTile* GridTile)
{
	if (CursorVisible)
	{
		Cursor->SetActorLocation(GridTile->GetActorLocation() + Cursor_ExtraHeight);
	}	
}

void APlayerController_TurnBased::EmptyStack()
{
	while(!StateStack.IsEmpty())
	{
		UControllerState_Abstract* State = StateStack.Pop();
		State->OnExit();
	}
}

void APlayerController_TurnBased::SetBaseState(UControllerState_Abstract* InState)
{
	EmptyStack();	
	PushState(InState, false);
}

void APlayerController_TurnBased::PushState(UControllerState_Abstract* InState, bool bDoExit)
{
	if (!StateStack.IsEmpty())
	{
		if (bDoExit)
		{
			StateStack.Top()->OnExit();
		}
		else
		{
			// disable input from this state
			StateStack.Top()->OnDisable();
		}
	}
	
	InState->OnEnter(this, 2);
	StateStack.Push(InState);
}

void APlayerController_TurnBased::PopState()
{	
	auto State = StateStack.Pop();
	State->OnExit();

	if (!StateStack.IsEmpty())
	{
		// restart the now top state
        StateStack.Top()->OnEnter(this, 2);
	}
}

void APlayerController_TurnBased::PopPushState(UControllerState_Abstract* InState, bool bDoExit)
{
	// TODO: pop current state then apply new state on top
	// call OnEnter on bottom state?
}

void APlayerController_TurnBased::OnCombatStart(AGridUnit* InInstigator, AGridUnit* InTarget)
{
	SetShowMouseCursor(false);
	APawn_FollowCursor* Pawn_FollowCursor = Cast<APawn_FollowCursor>(GetPawn());
	if (Pawn_FollowCursor)
	{
		Pawn_FollowCursor->SetFollowTarget(InInstigator);
	}
}

void APlayerController_TurnBased::OnCombatEnd(const FCombatPrediction& InCombatPrediction)
{
	SetShowMouseCursor(true);
}

void APlayerController_TurnBased::OnFactionStart(FGameplayTag FactionTag, UGameEventTaskManager* TaskManager)
{
	if (FactionTag == TAG_TBCore_Faction_Player)
	{
		CursorVisible = true;
		
		// set controller on, Idle will enable the cursor...
		SetBaseState(UControllerState_Idle::Create());
	}
	else
	{
		CursorVisible = false;
		
		// turn of controller
		EmptyStack();		
		SetShowMouseCursor(false);

		// don't allow player to move cursor
		if (APawn_FollowCursor* FollowPawn = Cast<APawn_FollowCursor>(GetPawn()))
		{
			FollowPawn->SetCursorCanTick(false);
		}
	}
}

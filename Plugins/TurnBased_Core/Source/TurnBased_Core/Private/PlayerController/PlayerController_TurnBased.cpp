// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_TurnBased.h"
#include "Combat/CombatWorldSubsystem.h"
#include "Engine/StaticMeshActor.h"
#include "Grid/GridWorldSubsystem.h"
#include "Pawn/APawn_FollowCursor.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "PlayerController/ControllerState_Idle.h"
#include "PlayerController/ControllerState_Wait.h"
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

	// control cursor and pawn functions when faction changing and whether player turn
	if (UTurnWorldSubsystem* TurnWorldSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
	{
		TurnWorldSubsystem->OnFactionStart.AddUniqueDynamic(this, &ThisClass::OnFactionStart);
		TurnWorldSubsystem->OnFactionStartPost.AddUniqueDynamic(this, &ThisClass::OnFactionStartPost);
	}

	// create initial state idle
	// PushState(NewObject<UControllerState_Idle>(), false);
	SetBaseState(NewObject<UControllerState_Wait>());
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
	TileCursor = GetWorld()->SpawnActor(AStaticMeshActor::StaticClass(), &SpawnTransform, SpawnParameters);
	UStaticMeshComponent* StaticMeshComponent = Cast<AStaticMeshActor>(TileCursor)->GetStaticMeshComponent();
	StaticMeshComponent->SetStaticMesh(CursorMesh);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetAffectDistanceFieldLighting(false);
	StaticMeshComponent->SetAffectDynamicIndirectLighting(false);
	TileCursorScale = TileCursorScale.GetAbs();
	StaticMeshComponent->SetWorldScale3D(TileCursorScale);

	// start with cursor hidden
	ShowTileCursor(false);
}

void APlayerController_TurnBased::ShowTileCursor(bool bShow)
{
	TileCursorVisible = bShow;
	TileCursor->SetActorHiddenInGame(!bShow);
}

void APlayerController_TurnBased::UpdateCursor(AGridTile* GridTile)
{
	// only update if cursor visible
	if (TileCursorVisible)
	{
		TileCursor->SetActorLocation(GridTile->GetActorLocation() + Cursor_ExtraHeight);
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
	const auto State = StateStack.Pop();
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

void APlayerController_TurnBased::OnFactionStart(FGameplayTag FactionTag, UGameEventTaskManager* TaskManager)
{
	SetBaseState(UControllerState_Wait::Create());
}

void APlayerController_TurnBased::OnFactionStartPost(FGameplayTag FactionTag)
{
	// if player turn set idle
	if (FactionTag == TAG_TBCore_Faction_Player)
	{		
		SetBaseState(UControllerState_Idle::Create());
	}
}

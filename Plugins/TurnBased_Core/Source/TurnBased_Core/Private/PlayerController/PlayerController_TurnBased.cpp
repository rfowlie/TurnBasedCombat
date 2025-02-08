// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_TurnBased.h"
#include "Engine/StaticMeshActor.h"
#include "Grid/GridWorldSubsystem.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "PlayerController/ControllerState_Idle.h"
#include "Tile/GridTile.h"


void APlayerController_TurnBased::BeginPlay()
{
	Super::BeginPlay();

	// create cursor
	CreateCursor();
	
	// bind cursor update to grid tile hovered
	if (UGridWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Subsystem->OnGridTileHoveredStart.AddUniqueDynamic(this, &ThisClass::UpdateCursor);
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
	Cursor->SetActorLocation(GridTile->GetActorLocation() + Cursor_ExtraHeight);
}

void APlayerController_TurnBased::SetBaseState(UControllerState_Abstract* InState)
{
	while(!StateStack.IsEmpty())
	{
		UControllerState_Abstract* State = StateStack.Pop();
		State->OnExit();
	}

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
	else
	{
		PushState(UControllerState_Idle::Create(), false);
	}	
}

void APlayerController_TurnBased::PopPushState(UControllerState_Abstract* InState, bool bDoExit)
{
	// TODO: pop current state then apply new state on top
	// call OnEnter on bottom state?
}

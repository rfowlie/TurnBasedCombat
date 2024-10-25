// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/TurnBasedCombatPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Grid/GridManager.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateMove.h"
#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateWait.h"


class UEnhancedInputLocalPlayerSubsystem;
struct FEnhancedActionKeyMapping;

ATurnBasedCombatPlayerController::ATurnBasedCombatPlayerController()
{
	// enable mouse interaction
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = false;
}

void ATurnBasedCombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CreateDefaultInputMapping();
	AddInputMapping();
	
	ControllerStateIndex = 0;
	SetState(ControllerStates[ControllerStateIndex]);
}

void ATurnBasedCombatPlayerController::Initialize(UGridManager* InGridManager)
{
	GridManager = InGridManager;

	// TODO: figure out what needs to stop when a grid event is firing
	// GridManager->OnGridEventStart.AddLambda([this]()
	// {
	// 	RemoveInputMapping();
	// });
	// GridManager->OnGridEventEnd.AddLambda([this]()
	// {
	// 	AddInputMapping();
	// });
	
	GridManager->OnGridEventStart.AddDynamic(this, &ThisClass::RemoveInputMapping);
	GridManager->OnGridEventEnd.AddDynamic(this, &ThisClass::AddInputMapping);
	
	// Create States
	UStateWait* StateWait = NewObject<UStateWait>(this);
	ControllerStates.Add(StateWait);
	UStateMove* StateMove = NewObject<UStateMove>(this);
	ControllerStates.Add(StateMove);	
	StateMove->Initialize(GridManager);
}

void ATurnBasedCombatPlayerController::SetState(UAbstractPlayerControllerState* NewState)
{
	check(NewState);
	// check(ControllerStateCurrent);
	if (ControllerStateCurrent)
	{
		ControllerStateCurrent->Exit(this);
	}
	
	ControllerStateCurrent = NewState;
	ControllerStateCurrent->Enter(this, 2);
}

void ATurnBasedCombatPlayerController::CreateDefaultInputMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	check(EIC)
	
	InputMappingContext = NewObject<UInputMappingContext>(this);

	// pause
	IA_Pause = NewObject<UInputAction>(this);
	IA_Pause->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Pause = InputMappingContext->MapKey(IA_Pause, EKeys::Escape);
	EIC->BindAction(IA_Pause, ETriggerEvent::Started, this, &ATurnBasedCombatPlayerController::PauseMenu);

	// cycle mode
	IA_CycleMode = NewObject<UInputAction>(this);
	IA_CycleMode->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_CycleMode = InputMappingContext->MapKey(IA_CycleMode, EKeys::Tab);
	EIC->BindAction(IA_CycleMode, ETriggerEvent::Started, this, &ATurnBasedCombatPlayerController::CycleMode);

	// move camera
	IA_MoveCamera = NewObject<UInputAction>(this);
	IA_MoveCamera->ValueType = EInputActionValueType::Axis2D;
	FEnhancedActionKeyMapping& Mapping_Move_Right = InputMappingContext->MapKey(IA_MoveCamera, EKeys::D);
	FEnhancedActionKeyMapping& Mapping_Move_Left = InputMappingContext->MapKey(IA_MoveCamera, EKeys::A);
	Mapping_Move_Left.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	FEnhancedActionKeyMapping& Mapping_Move_Up = InputMappingContext->MapKey(IA_MoveCamera, EKeys::W);
	Mapping_Move_Up.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(this));
	FEnhancedActionKeyMapping& Mapping_Move_Down = InputMappingContext->MapKey(IA_MoveCamera, EKeys::S);
	Mapping_Move_Down.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	Mapping_Move_Down.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(this));
	EIC->BindAction(IA_MoveCamera, ETriggerEvent::Started, this, &ATurnBasedCombatPlayerController::Controls);
}

void ATurnBasedCombatPlayerController::AddInputMapping()
{
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(EILP)
	EILP->AddMappingContext(InputMappingContext, 0);
}

void ATurnBasedCombatPlayerController::RemoveInputMapping()
{
	UEnhancedInputLocalPlayerSubsystem* EILP = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(EILP)
	EILP->RemoveMappingContext(InputMappingContext);
}

void ATurnBasedCombatPlayerController::PauseMenu()
{
	//...
}

void ATurnBasedCombatPlayerController::CycleMode()
{
	// TODO: either remove input mapping context that has this or insert way to prevent this from firing
	// will be needed when we start introducing inbetween states or phases of play like watching an animation play out
	
	check(!ControllerStates.IsEmpty())
	check(ControllerStateCurrent)

	// currently this way as we might be using in between states like wait/pause which are not a part of the
	// normal modes that you can cycle through
	if (ControllerStates.Contains(ControllerStateCurrent))
	{
		int32 Index = 0;
		ControllerStates.Find(ControllerStateCurrent, Index);
		Index = (Index + 1 + ControllerStates.Num()) % ControllerStates.Num();
		SetState(ControllerStates[Index]);
	}
	else
	{
		// sets wait...
		SetState(ControllerStates[0]);
	}
}

void ATurnBasedCombatPlayerController::Controls()
{
	UE_LOG(LogTemp, Error, TEXT("PC Input"));
}

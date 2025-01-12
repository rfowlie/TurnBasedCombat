// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/_Framework/PlayerController/State/StateAttack.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Combat/DuelContainer.h"
#include "Grid/Manager/GridManager.h"
#include "Grid/GridProxy.h"
#include "_Framework/TBC_InfoWorldSubsystem.h"
#include "_Framework/HUD/TurnBasedCombatHUD.h"
#include "_Framework/PlayerController/UI/CombatDisplayWidget.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack, "Encounter.Mode.Attack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack_None, "Encounter.Mode.Attack.None");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack_Idle, "Encounter.Mode.Attack.Idle");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack_SelectedAttacker, "Encounter.Mode.Attack.SelectedAttacker");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack_SelectedTarget, "Encounter.Mode.Attack.SelectedTarget");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack_SelectedAttackTile, "Encounter.Mode.Attack.SelectedAttackTile");
UE_DEFINE_GAMEPLAY_TAG(TAG_Encounter_Mode_Attack_Combat, "Encounter.Mode.Attack.Combat");


UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Attacker, "Grid.State.Attacker");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_CanTargetFrom, "Grid.State.CanTargetFrom");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_Target, "Grid.State.Target");
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_State_TargetFrom, "Grid.State.TargetFrom");

// UI
UE_DEFINE_GAMEPLAY_TAG(TAG_UI_Combat_DuelInfo, "UI.Combat.DuelInfo");


UStateAttack::UStateAttack()
{
}

void UStateAttack::Initialize(UGridManager* InGridManager, AHUD* InTurnBasedCombatHUD)
{
	GridManager = InGridManager;
	GridManager->OnGridEventStart.AddUObject(this, &ThisClass::Disable);
	GridManager->OnGridEventEnd.AddUObject(this, &ThisClass::Enable);

	if (ATurnBasedCombatHUD* TurnBasedCombatHUD = Cast<ATurnBasedCombatHUD>(InTurnBasedCombatHUD))
	{
		HUD = TurnBasedCombatHUD;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("State Attack: HUD not of type ATurnBasedCombatHUD"));
	}
}

FGameplayTag UStateAttack::GetStateTag() const
{
	return TAG_Encounter_Mode_Attack;
}

void UStateAttack::SetPhase(FGameplayTag InTagPhase)
{
	TagPhase = InTagPhase;
	if (UTBC_InfoWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UTBC_InfoWorldSubsystem>())
	{
		Subsystem->SetPlayerControllerPhase(InTagPhase);
	}
}

UInputMappingContext* UStateAttack::SetupInputMappingContext(APlayerController* PlayerController)
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EIC)
	
	UInputMappingContext* NewInputMappingContext = NewObject<UInputMappingContext>(this);

	IA_Select = NewObject<UInputAction>(this);
	IA_Select->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Select = NewInputMappingContext->MapKey(IA_Select, EKeys::LeftMouseButton);
	EIC->BindAction(IA_Select, ETriggerEvent::Started, this, &UStateAttack::OnSelect);
	
	IA_Deselect = NewObject<UInputAction>(this);
	IA_Deselect->ValueType = EInputActionValueType::Boolean;
	FEnhancedActionKeyMapping& Mapping_Deselect = NewInputMappingContext->MapKey(IA_Deselect, EKeys::RightMouseButton);
	EIC->BindAction(IA_Deselect, ETriggerEvent::Started, this, &UStateAttack::OnDeselect);
	
	IA_Cycle = NewObject<UInputAction>(this);
	IA_Cycle->ValueType = EInputActionValueType::Axis1D;
	FEnhancedActionKeyMapping& Mapping_Cycle_Right = NewInputMappingContext->MapKey(IA_Cycle, EKeys::E);
	FEnhancedActionKeyMapping& Mapping_Cycle_Left = NewInputMappingContext->MapKey(IA_Cycle, EKeys::Q);
	Mapping_Cycle_Left.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	EIC->BindAction(IA_Cycle, ETriggerEvent::Started, this, &UStateAttack::OnCycleUnit);
	
	return NewInputMappingContext;
}

void UStateAttack::OnEnter()
{	
	UE_LOG(LogTemp, Warning, TEXT("OnEnter - ATTACK STATE"));
	SetPhase(TAG_Encounter_Mode_Attack_Idle);
}

void UStateAttack::OnExit()
{
	Super::OnExit();

	if (HUD)
	{
		HUD->ToggleUI(TAG_UI_Combat_DuelInfo, false);
	}

	UndoAll();
}

void UStateAttack::OnSelect()
{
	UGridProxy* NewGridProxy = GridManager->GetCurrentHoveredGridTile();
	if (!NewGridProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridProxy is null"));
		return;
	}
	
	if (TagPhase == TAG_Encounter_Mode_Attack_Idle)
	{
		// check if previous selection, and undo changes
		if (Instigator)
		{
			Instigator->UndoAll();
		}
		// set new selection
		Instigator = NewGridProxy;
		if (Instigator->IsAlly() && Instigator->HasEnemiesToAttack())
		{
			Instigator->SetEnemiesInRange(true);
			SetPhase(TAG_Encounter_Mode_Attack_SelectedAttacker);
			if (UTBC_InfoWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UTBC_InfoWorldSubsystem>())
			{
				Subsystem->SetGridUnitSelected(Instigator->GridUnit);
			}
		}
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_SelectedAttacker)
	{
		// check if new player unit selected
		// if (GridProxyCurrent != GridProxy && GridProxy->IsAlly())
		// {
		// 	GridProxyCurrent->UndoAll();
		// 	GridProxyCurrent = GridProxy;
		// 	GridProxyCurrent->SetEnemiesInRange(true);
		// }
		// check if target is enemy that can be attacked, move to next phase
		if (Instigator->CanAttack(NewGridProxy))
		{
			if (Target) { Target->UndoAll(); }
			Instigator->SetCanTargetFromTiles(Target, false);
			Target = NewGridProxy;
			Instigator->SetCanTargetFromTiles(Target, true);
			SetPhase(TAG_Encounter_Mode_Attack_SelectedTarget);
		}
		// check if new enemy target selected
		// if (GridProxyTarget != GridProxy)
		// {
		// 	if (GridProxyTarget) { GridProxyTarget->UndoAll(); }
		// 	GridProxyTarget = GridProxy;
		// }		
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_SelectedTarget)
	{
		// check if new player unit selected, return to previous phase
		// if (GridProxyCurrent != GridProxy && GridProxy->IsAlly())
		// {
		// 	GridProxyCurrent->UndoAll();
		// 	GridProxyCurrent = GridProxy;
		// 	GridProxyCurrent->SetEnemiesInRange(true);
		// 	Phase = EAttackPhase::SelectedAttacker;
		// }
		// check if new enemy target selected, stay on phase
		// else if (GridProxyTarget != GridProxy && GridProxyCurrent->CanAttack(GridProxy) && !GridProxy->IsAlly())
		// {
		// 	GridProxyCurrent->SetCanTargetFromTiles(GridProxyTarget, false);
		// 	GridProxyTarget->UndoAll();
		// 	GridProxyTarget = GridProxy;
		// 	GridProxyCurrent->SetCanTargetFromTiles(GridProxyTarget, true);
		// }
		// check if tile can attack from is selected
		if (Instigator->CanAttackFromTile(NewGridProxy))
		{
			UDuelContainer* DuelContainer = UDuelContainer::CreateContainer(
				Instigator->GridUnit, Instigator->GridTile, Target->GridUnit, Target->GridTile);
			HUD->UpdateUI(TAG_UI_Combat_DuelInfo, DuelContainer);
			HUD->ToggleUI(TAG_UI_Combat_DuelInfo, true);

			if (MovingTo) { MovingTo->UndoAll(); }
			MovingTo = NewGridProxy;
			MovingTo->SetMoveToTile(true);
			SetPhase(TAG_Encounter_Mode_Attack_SelectedAttackTile);
		}
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_SelectedAttackTile)
	{
		// check if other tile can attack from is selected, stay on phase
		// if (GridProxyMoveTo != GridProxy && GridProxyTarget->CanAttackFromTile(GridProxy))
		// {
		// 	if (GridProxyMoveTo) { GridProxyMoveTo->UndoAll(); }
		// 	GridProxyMoveTo = GridProxy;
		// 	GridProxyMoveTo->SetMoveToTile(true);
		// }
		// TODO: for now hackyyy
		// check if same tile is selected to confirm attack
		// if (GridManager->IsMatch(MovingTo ,NewGridProxy))
		if (GridManager->IsMatch(MovingTo, NewGridProxy))
		{
			GridManager->CreateAttackEvent(Instigator, Target, MovingTo);
		}
		// THIS IS THE OLD WAY USING CUSTOM MADE EVENT SYSTEM INSTEAD OF Gameplay Ability System
		// {
		// 	TArray<UAbstractEvent*> Events;
		// 	UEventUnitMove* EventMove = NewObject<UEventUnitMove>(this);
		// 	UEventUnitAttack* EventAttack = NewObject<UEventUnitAttack>(this);
		// 	Events.AddUnique(EventMove);
		// 	Events.AddUnique(EventAttack);
		// 	OnEventCreate.Execute(Events);
		// }
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_Combat)
	{
		// do nothing...
	}

	UE_LOG(LogTemp, Error, TEXT("Mode: Attack - On Select, Phase: %ls"), *TagPhase.ToString());
}

void UStateAttack::OnDeselect()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeselect - ATTACK"));
	
	if (TagPhase == TAG_Encounter_Mode_Attack_Idle)
	{
		if (UTBC_InfoWorldSubsystem* Subsystem = GetWorld()->GetSubsystem<UTBC_InfoWorldSubsystem>())
		{
			Subsystem->SetGridUnitSelected(nullptr);
		}
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_SelectedAttacker)
	{
		UndoSelectedAttacker();
		SetPhase(TAG_Encounter_Mode_Attack_Idle);
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_SelectedTarget)
	{
		UndoSelectedTarget();
		SetPhase(TAG_Encounter_Mode_Attack_SelectedAttacker);
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_SelectedAttackTile)
	{
		UndoSelectedAttackTile();
		SetPhase(TAG_Encounter_Mode_Attack_SelectedTarget);
	}
	else if (TagPhase == TAG_Encounter_Mode_Attack_Combat)
	{
		// do nothing...
	}
}

void UStateAttack::OnCycleUnit()
{
	UndoAll();
	
	Instigator = GridManager->GetNextGridUnit(Instigator);
	Instigator->SetEnemiesInRange(true);
	SetPhase(TAG_Encounter_Mode_Attack_SelectedAttacker);
}

void UStateAttack::UndoAll()
{
	UndoSelectedAttackTile();
	UndoSelectedTarget();
	UndoSelectedAttacker();

	SetPhase(TAG_Encounter_Mode_Attack_Idle);
}

void UStateAttack::UndoSelectedAttacker()
{
	if (Instigator) { Instigator->SetEnemiesInRange(false); }
	Instigator = nullptr;
}

void UStateAttack::UndoSelectedTarget()
{
	if (Target) { Target->SetEnemyTargetTile(false); }
	Target = nullptr;

	HUD->ToggleUI(TAG_UI_Combat_DuelInfo.GetTag(), false);
}

void UStateAttack::UndoSelectedAttackTile()
{
	if (MovingTo) { MovingTo->SetMoveToTile(false); }
	MovingTo = nullptr;
}

void UStateAttack::Enable()
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackState - Enable"));
	if (Instigator)
	{
		Instigator->SetMoveableTiles(false);
		Instigator = nullptr;
	}
	if (Target)
	{
		Target->UndoAll();
		Target = nullptr;
	}
	if (MovingTo)
	{
		MovingTo->UndoAll();
		MovingTo = nullptr;
	}
		
	SetPhase(TAG_Encounter_Mode_Attack_Idle);
}

void UStateAttack::Disable()
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackState - Disable"));
	SetPhase(TAG_Encounter_Mode_Attack_None);
}

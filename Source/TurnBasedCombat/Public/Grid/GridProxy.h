// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GridProxy.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Mover);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Moveable);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_MoveTo);


struct FGridPosition;
struct FGridMovement;
class UTurnManager;
class AGridUnit;
class AGridTile;

// TArray<FGridMovement> UGridManager::CalculateGridMovement(AGridUnit* GridUnit)
DECLARE_DELEGATE_TwoParams(FCalculateGridMovement, TArray<FGridMovement>&, AGridUnit*);

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UGridProxy : public UObject
{
	GENERATED_BODY()

public:
	// Factory method
	static UGridProxy* CreateGridProxy(
		UObject* Outer,
		UTurnManager* InTurnManager,
		AGridTile* GridTile,
		AGridUnit* GridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements);
	
	void UndoAll();
	void SetMoveableTiles(bool Activate);
	void SetTargetableEnemies(bool Activate);
	void SetMoveToTile(bool Activate);
	void SetEnemyTargetTile(bool Activate);
	void SetAttackTiles(UGridProxy* GridProxy, bool Activate);

	bool CanMoveToo(UGridProxy* GridProxy);
	bool CanAttack(UGridProxy* GridProxy);

	TArray<int32> GetAttackRanges();

	bool IsAlly(UGridProxy* GridProxy);
	bool IsPlayer();
	bool IsEnemy();
	bool IsMoveTile(UGridProxy* GridProxy);
	bool IsAttackTile(UGridProxy* GridProxy);

protected:
	UPROPERTY()
	UTurnManager* TurnManager = nullptr;	
	UPROPERTY()
	AGridTile* GridTile = nullptr;	
	UPROPERTY()
	AGridUnit* GridUnit = nullptr;

	// TODO: do not do calculations in here
	// pass all relevant/required information to the proxy and then create strict
	// functions to execute desired actions (ex. highlight attackable tiles)
	UPROPERTY()
	TArray<FGridMovement> GridMovements;
	UPROPERTY()
	TArray<FGridPosition> AttackFrom;

	

private:
	// force factory method to create this class
	UGridProxy();
	
	virtual void Init(
		UTurnManager* InTurnManager,
		AGridTile* InGridTile,
		AGridUnit* InGridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements);
	
	FCalculateGridMovement CalculateGridMovementDelegate;
};

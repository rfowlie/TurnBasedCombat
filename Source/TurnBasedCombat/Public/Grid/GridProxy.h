// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GridProxy.generated.h"


struct FGridPair;
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Encounter_Mode_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Idle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Mover);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Moveable);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_MoveTo);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_CanAttack);



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

	friend class UGridManager;

public:
	// Factory method
	static UGridProxy* CreateGridProxy(
		UObject* Outer,
		UTurnManager* InTurnManager,
		AGridTile* GridTile,
		AGridUnit* GridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements,
		TArray<FGridPair> InEnemyUnitsInRange);
	
	// void SetState(FGameplayTag State);
	
	void UndoAll();
	void SetMoveableTiles(bool Activate);
	void SetEnemiesInRange(bool Activate);
	void SetMoveToTile(bool Activate);
	void SetEnemyTargetTile(bool Activate);
	bool SetCanTargetFromTiles(UGridProxy* GridProxy, bool Activate);

	bool CanMoveToo(UGridProxy* GridProxy);
	bool CanAttack(UGridProxy* GridProxy);

	// TArray<int32> GetAttackRanges();

	bool HasUnit() const;
	bool IsAlly() const;
	bool IsMoveTile(UGridProxy* Other);
	bool CanAttackFromTile(UGridProxy* Other) const;
	bool HasEnemiesToAttack() const;

	FVector GetWorldLocation() const;
	FGridPosition GetGridPosition() const;
	FGameplayTag GetFaction() const;
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
	TArray<FGridPair> EnemyGridUnitsInRange;
	UPROPERTY()
	TArray<FGridMovement> CurrentCanAttackFromTiles;

private:
	// force factory method to create this class
	UGridProxy();
	
	virtual void Init(
		UTurnManager* InTurnManager,
		AGridTile* InGridTile,
		AGridUnit* InGridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements,
		TArray<FGridPair> InEnemyUnitsInRange);
	
	FCalculateGridMovement CalculateGridMovementDelegate;
};

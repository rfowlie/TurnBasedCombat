// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GridProxy.generated.h"


class UAbilitySystemComponent;
class UWeaponDataAsset_OLD;
class UTurnManager;
class AGridUnit;
class AGridTile;
struct FCombatDisplayInfo;
struct FGridPair;
struct FGridPosition;
struct FGridMovement;


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Idle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Mover);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_Moveable);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_MoveTo);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Grid_State_CanAttack);


// TArray<FGridMovement> UGridManager::CalculateGridMovement(AGridUnit* GridUnit)
DECLARE_DELEGATE_TwoParams(FCalculateGridMovement, TArray<FGridMovement>&, AGridUnit*);

/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASEDCOMBAT_API UGridProxy : public UObject
{
	GENERATED_BODY()

	friend class UGridManager;
	
	// force factory method to create this class
	UGridProxy();

public:
	// NOT WORKING???
	// bool operator==(const UGridProxy& Other) const
	// {
	// 	return GridTile == Other.GridTile && GridUnit == Other.GridUnit;
	// }

	// Factory method
	static UGridProxy* CreateGridProxy(
		UObject* Outer,
		UTurnManager* InTurnManager,
		AGridTile* GridTile,
		AGridUnit* GridUnit,
		const FCalculateGridMovement& InMovementDelegate,
		TArray<FGridMovement> InGridMovements,
		TArray<FGridPair> InEnemyUnitsInRange);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
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
	bool GetValidWeaponsToAttackWith(UGridProxy* Other, TArray<UWeaponDataAsset_OLD*> ValidWeapons) const;
	bool HasEnemiesToAttack() const;

	FVector GetWorldLocation() const;
	FGridPosition GetGridPosition() const;
	FGameplayTag GetFaction() const;

	// TODO: for now!!
	UPROPERTY(BlueprintReadOnly)
	AGridTile* GridTile = nullptr;	
	UPROPERTY(BlueprintReadOnly)
	AGridUnit* GridUnit = nullptr;
	
protected:
	UPROPERTY()
	UTurnManager* TurnManager = nullptr;	

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
	// virtual void Init(
	// 	UTurnManager* InTurnManager,
	// 	AGridTile* InGridTile,
	// 	AGridUnit* InGridUnit,
	// 	const FCalculateGridMovement& InMovementDelegate,
	// 	TArray<FGridMovement> InGridMovements,
	// 	TArray<FGridPair> InEnemyUnitsInRange);
	
	FCalculateGridMovement CalculateGridMovementDelegate;
};

﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Grid/GridProxy.h"
#include "Grid/GridStructs.h"
#include "Grid/Unit/GridUnit.h"
#include "GridManager.generated.h"


class UGameplayAbility;
class UTurnManager;
class AGridTile;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Attack);

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDisplayTile, UTerrainDataAsset*, DataAsset, FTileStatsSnapshot, Snapshot, const FName, TerrainType);
// TODO: do not pass GridProxy, create new class or interface to pass out
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridHovered, UGridProxy*, GridProxy);
// TODO: probably want to change to const reference if not going to change these parameters...
DECLARE_MULTICAST_DELEGATE_OneParam(FGridTileHovered, const AGridTile*);
DECLARE_MULTICAST_DELEGATE_OneParam(FGridUnitHovered, const AGridUnit*);
DECLARE_MULTICAST_DELEGATE(FGridManagerDelegate);


UCLASS()
class TURNBASEDCOMBAT_API UGridManager : public UObject
{
	GENERATED_BODY()

public:
	UGridManager();

	void Initialize(UTurnManager* InurnManager);

	FGridTileHovered OnGridTileHovered;	
	FGridUnitHovered OnGridUnitHovered;
	void RegisterGridTile(AGridTile* GridTile);
	void RegisterGridUnit(AGridUnit* GridUnit);	

	// TODO: rename this, make it make sense
	UGridProxy* GetCurrentHoveredGridTile();
	UGridProxy* GetNextGridUnit(UGridProxy* InGridProxy, bool Next = true);
	
	void CreateMoveEvent(UGridProxy* Instigator, UGridProxy* Location);
	UFUNCTION()
	void PostEvent_Move(AGridUnit* GridUnit);
	
	void CreateAttackEvent(
		UGridProxy* Instigator, UGridProxy* Location, UGridProxy* Target);
	UFUNCTION()
	void PostEvent_Attack(AGridUnit* GridUnit);

	UFUNCTION()
	void OnEndEvent();
	
	FGridManagerDelegate OnGridEventStart;
	FGridManagerDelegate OnGridEventEnd;

	// TODO: determine best way to do this
	bool IsMatch(const UGridProxy* GridProxy_A, const UGridProxy* GridProxy_B);

protected:
	UFUNCTION()
	void OnBeginCursorOverGridTile(AActor* Actor);

	UPROPERTY()
	AGridTile* CurrentHoveredTile = nullptr;
	UPROPERTY()
	AGridUnit* CurrentHoveredUnit = nullptr;

private:
	UPROPERTY()
	UTurnManager* TurnManager = nullptr;
	
	UPROPERTY()
	TArray<AGridTile*> GridTilesAll;
	UPROPERTY()
	TMap<FGridPosition, AGridTile*> LocationGridTileMap;
	UPROPERTY()
	TArray<AGridUnit*> GridUnitsAll;
	UPROPERTY()
	TMap<FGridPosition, AGridUnit*> LocationGridUnitMap;
	UPROPERTY()
	TMap<AGridUnit*, FGridPosition> GridUnitLocationMap;

	TArray<FGridMovement> CalculateGridMovement(AGridUnit* GridUnit);
	void CalculateGridAttacks(TArray<const AGridUnit*> OutGridUnitsInRange, AGridUnit* GridUnit);
	TArray<FGridPair> CalculateGridAttacks(AGridUnit* GridUnit);
	TArray<FTargetingUnit> CalculateGridTargets(AGridUnit* GridUnit);
	void GetEnemyUnits(TArray<AGridUnit*>& EnemyGridUnits, AGridUnit* GridUnit);
	TArray<FGridPosition> GetEnemyPositions(const AGridUnit* GridUnit) const;
	
	// difference from utils is that this returns AGridTile instead of FGridPositio
	TArray<AGridTile*> GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range);
	
	TArray<AGridTile*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);

	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	TArray<AGridTile*> GetGridTilesInRange(const FGridPosition StartGridPosition, int32 Range);
	
};

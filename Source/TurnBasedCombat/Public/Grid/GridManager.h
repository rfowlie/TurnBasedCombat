// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridProxy.h"
#include "GridStructs.h"
#include "Abilities/GameplayAbility.h"
#include "Unit/GridUnit.h"
#include "GridManager.generated.h"


/**
 * 
 */

class UGameplayAbility;
class UTurnManager;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDisplayTile, UTerrainDataAsset*, DataAsset, FTileStatsSnapshot, Snapshot, const FName, TerrainType);
// TODO: do not pass GridProxy, create new class or interface to pass out
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridHovered, UGridProxy*, GridProxy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridManagerDelegate);

UCLASS()
class TURNBASEDCOMBAT_API UGridManager : public UObject
{
	GENERATED_BODY()

public:
	UGridManager();
	
	void RegisterGridTile(AGridTile* GridTile);
	void RegisterGridUnit(AGridUnit* GridUnit);

	UPROPERTY(BlueprintAssignable)
	FOnGridHovered OnGridTileHovered;

	// TODO: rename this, make it make sense
	UGridProxy* GetCurrentHoveredGridTile();
	UGridProxy* GetNextGridUnit(UGridProxy* InGridProxy, bool Next = true);
	// bool CreateMoveEvent(UGridProxy* Instigator, UGridProxy* Location, const TMulticastDelegate<void(UGameplayAbility*)>::FDelegate& Callback);
	void CreateMoveEvent(UGridProxy* Instigator, UGridProxy* Location);
	UFUNCTION()
	void PostMoveEvent(AGridUnit* GridUnit);
	bool CreateAttackEvent(UGridProxy* Instigator, UGridProxy* Target, UGridProxy* Location);

	UFUNCTION()
	void OnEndEvent();

	// DECLARE_EVENT(UGridManager, FGridManagerEvent)
	// FGridManagerEvent OnGridEventStart;
	// FGridManagerEvent OnGridEventEnd;
	
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
	void CalculateGridAttacks(TArray<FGridPosition>& OutGridPositions, AGridUnit* GridUnit);
	TArray<FTargetingUnit> CalculateGridTargets(AGridUnit* GridUnit);
	TArray<FGridPosition> GetEnemyPositions(const AGridUnit* GridUnit) const;
	
	// difference from utils is that this returns AGridTile instead of FGridPositio
	TArray<AGridTile*> GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range);
	
	TArray<AGridTile*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);

	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	TArray<AGridTile*> GetGridTilesInRange(const FGridPosition StartGridPosition, int32 Range);
	
};

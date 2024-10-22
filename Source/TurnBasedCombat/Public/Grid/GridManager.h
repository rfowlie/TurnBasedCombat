// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridProxy.h"
#include "GridStructs.h"
#include "GridManager.generated.h"


/**
 * 
 */

class UTurnManager;
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDisplayTile, UTerrainDataAsset*, DataAsset, FTileStatsSnapshot, Snapshot, const FName, TerrainType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridHovered, UGridProxy*, GridProxy);

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

	UGridProxy* GetCurrentHoveredGridTile();
	UGridProxy* GetNextGridUnit(UGridProxy* InGridProxy, bool Next = true);

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
	void CalculateGridMovementAttackable(TArray<FGridPosition>& OutGridPositions, AGridUnit* GridUnit);
	TArray<FTargetingUnit> CalculateGridTargets(AGridUnit* GridUnit);
	TArray<FGridPosition> GetEnemyPositions(const AGridUnit* GridUnit) const;
	
	// difference from utils is that this returns AGridTile instead of FGridPositio
	TArray<AGridTile*> GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range);
	
	TArray<AGridTile*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);

	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	TArray<AGridTile*> GetGridTilesInRange(const FGridPosition StartGridPosition, int32 Range);
	
};

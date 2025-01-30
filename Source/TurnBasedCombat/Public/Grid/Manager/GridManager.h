// Fill out your copyright notice in the Description page of Project Settings.

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
class UGridRules;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Death);

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

	void Initialize(UGridRules* InGridRules, UTurnManager* InTurnManager);

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
	void OnEndEvent() const;
	
	FGridManagerDelegate OnGridEventStart;
	FGridManagerDelegate OnGridEventEnd;
	// TArray<TPair<AGridUnit*, UGameplayAbility*>> GridUnitsTakingActions;
	TArray<TPair<AActor*, UGameplayAbility*>> GridUnitsTakingActions;
	UFUNCTION()
	void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);
	UFUNCTION()
	void OnGridUnitAbilityEnded(UGameplayAbility* InGameplayAbility);
	// UFUNCTION()
	// void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility, AGridUnit* InGridUnit);
	// UFUNCTION()
	// void OnGridUnitAbilityEnded(UGameplayAbility* InGameplayAbility, AGridUnit* InGridUnit);

	// this is hacky...
	bool IsMatch(const UGridProxy* GridProxy_A, const UGridProxy* GridProxy_B);

protected:
	UFUNCTION()
	void OnBeginCursorOverGridTile(AActor* Actor);

	UPROPERTY()
	AGridTile* CurrentHoveredTile = nullptr;
	UPROPERTY()
	AGridUnit* CurrentHoveredUnit = nullptr;

	UPROPERTY()
	UGridRules* GridRules = nullptr;
	UPROPERTY()
	UTurnManager* TurnManager = nullptr;
	
	UPROPERTY()
	TArray<AGridTile*> GridTilesAll;
	UPROPERTY()
	TMap<FGridPosition, AGridTile*> LocationGridTileMap;
	UPROPERTY()
	TMap<AGridTile*, FGridPosition> GridTileLocationMap;
	UPROPERTY()
	TArray<AGridUnit*> GridUnitsAll;
	UPROPERTY()
	TMap<FGridPosition, AGridUnit*> LocationGridUnitMap;
	UPROPERTY()
	TMap<AGridUnit*, FGridPosition> GridUnitLocationMap;

	void UpdateTileMapping(AGridTile* GridTile);
	void UpdateUnitMapping(AGridUnit* GridUnit);
	void UpdateUnitMappingsAll();
	
public:
	AGridUnit* GetGridUnitOnTile(const AGridTile* GridTile) const
	{
		if (GridTileLocationMap.Contains(GridTile))
		{
			if (LocationGridUnitMap.Contains(GridTileLocationMap[GridTile]))
			{
				return LocationGridUnitMap[GridTileLocationMap[GridTile]];
			}
		}

		return nullptr;
	}
	
	AGridTile* GetGridTileOfUnit(AGridUnit* GridUnit)
	{
		UpdateUnitMapping(GridUnit);
		if (GridUnitLocationMap.Contains(GridUnit))
		{
			if (LocationGridTileMap.Contains(GridUnitLocationMap[GridUnit]))
			{
				return LocationGridTileMap[GridUnitLocationMap[GridUnit]];
			}
		}
		
		return nullptr;
	}

protected:
	void CalculateGridMovement(TArray<FGridMovement>& OutMovement, AGridUnit* GridUnit);
	void CalculateGridAttacks(TArray<const AGridUnit*> OutGridUnitsInRange, AGridUnit* GridUnit);
	TArray<FGridPair> CalculateGridAttacks(AGridUnit* GridUnit);
	TArray<FTargetingUnit> CalculateGridTargets(AGridUnit* GridUnit);
	void GetEnemyUnits(TArray<AGridUnit*>& EnemyGridUnits, AGridUnit* GridUnit);
	TArray<FGridPosition> GetEnemyPositions(const AGridUnit* GridUnit) const;
	
	// difference from utils is that this returns AGridTile instead of FGridPosition
	TArray<AGridTile*> GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range);
	
	TArray<AGridTile*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);

	// return all grid coordinates that fall at or within the range
	UFUNCTION(BlueprintCallable)
	TArray<AGridTile*> GetGridTilesInRange(const FGridPosition StartGridPosition, int32 Range);
	
};

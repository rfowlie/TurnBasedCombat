// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridMechanics_Structs.h"
#include "GigafireGridInterpreterComponent.generated.h"


class AGridTileBase;
class AGridUnitBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GIGAFIRE_API UGigafireGridInterpreterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GridSize = 200.f;
	
	// UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatCalculator")
	// const UCombatCalculator_Basic* GetCombatCalculator();

#pragma region Tile	
	UPROPERTY()
	TMap<AGridTileBase*, FGridPosition> GridTileLocationMap;
	UPROPERTY()
	TMap<FGridPosition, AGridTileBase*> LocationGridTileMap;

	// gets tiles from GridTrackerWorldSubsystem
	UFUNCTION(BlueprintCallable)
	void UpdateTileMappingsAll();
	UFUNCTION(BlueprintCallable)
	void UpdateTileMapping(AGridTileBase* GridTile);
	
#pragma endregion Tile

	
#pragma region Unit
	UPROPERTY()
	TMap<AGridUnitBase*, FGridPosition> GridUnitLocationMap;
	UPROPERTY()
	TMap<FGridPosition, AGridUnitBase*> LocationGridUnitMap;

	// gets units from GridTrackerWorldSubsystem
	UFUNCTION(BlueprintCallable)
	void UpdateUnitMappingsAll();
	UFUNCTION(BlueprintCallable)
	void UpdateUnitMapping(AGridUnitBase* GridUnit);
	
#pragma endregion Unit


#pragma region Calculations
	UFUNCTION(BlueprintCallable)
	AGridUnitBase* GetGridUnitOnTile(const AGridTileBase* GridTile) const;
	UFUNCTION(BlueprintCallable)
	AGridTileBase* GetGridTileOfUnit(const AGridUnitBase* GridUnit) const;
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanUnitBeOnTile(AGridUnitBase* GridUnit, AGridTileBase* GridTile);
	// TODO: Movement policy system, this is janky...
	virtual int32 InterpretMovementCost(AGridUnitBase* GridUnit, AGridTileBase* GridTile);
	
	TArray<AGridTileBase*> GetGridTilesAtRange(const FGridPosition StartGridPosition, int32 Range);
	TArray<AGridTileBase*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);

	// TODO: is this where these go???
	virtual void CalculateGridMovement(TArray<FGridMovement>& OutMovement, AGridUnitBase* GridUnit, const int32 AvailableMovement);
	virtual void CalculateGridAttacks(TArray<FGridPair>& OutGridPairs, AGridUnitBase* InGridUnit, const TArray<FGridMovement>& InGridMovements);
	virtual void CalculateGridAttackTiles(TMap<AGridTileBase*, int32>& OutWeaponPositions, const TArray<FGridMovement>& InGridMovements, const AGridUnitBase* InstigatorUnit, const AGridUnitBase* TargetUnit);
#pragma endregion 
};

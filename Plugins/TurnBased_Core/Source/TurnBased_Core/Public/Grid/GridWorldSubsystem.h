// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridStructs.h"
#include "GridWorldSubsystem.generated.h"

class UGridRules;
class UGameplayAbility;
class AGridUnit;
class AGridTile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridTileDelegate, AGridTile*, GridTile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridUnitDelegate, AGridUnit*, GridUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridUnitAbilityDelegate);

/**
 * organize, manage and broadcast all things related to the grid
 */
UCLASS()
class TURNBASED_CORE_API UGridWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// EVENTS	
	// tile
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridTileDelegate OnGridTileHoveredStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridTileDelegate OnGridTileHoveredStop;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridTileDelegate OnGridTileSelectedStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridTileDelegate OnGridTileSelectedStop;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridTileDelegate OnGridTileChanged;

	// unit
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitDelegate OnGridUnitHoveredStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitDelegate OnGridUnitHoveredStop;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitDelegate OnGridUnitSelectedStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitDelegate OnGridUnitSelectedStop;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitDelegate OnGridUnitChanged;

	
	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void RegisterGridTile(AGridTile* GridTile);
	UFUNCTION(BlueprintCallable)
	void RegisterGridUnit(AGridUnit* GridUnit);
	UFUNCTION()
	void OnBeginCursorOverGridTile(AActor* Actor);

	// VARIABLES
	// tile
	UPROPERTY()
	TArray<AGridTile*> GridTilesAll;
	UPROPERTY()
	TMap<AGridTile*, FGridPosition> GridTileLocationMap;
	UPROPERTY()
	TMap<FGridPosition, AGridTile*> LocationGridTileMap;
	UPROPERTY()
	AGridTile* GridTileHovered = nullptr;
	UFUNCTION(BlueprintCallable)
	AGridTile* GetGridTileHovered() const { return GridTileHovered; }
	UPROPERTY()
	AGridTile* GridTileSelected = nullptr;
	// AGridTile* GetGridTileSelected() const { return GridTileSelected; }
	
	void UpdateTileMapping(AGridTile* GridTile);
	void UpdateTileMappingsAll();
	
	// unit
	UPROPERTY()
	TArray<AGridUnit*> GridUnitsAll;
	UPROPERTY()
	TMap<AGridUnit*, FGridPosition> GridUnitLocationMap;
	UPROPERTY()
	TMap<FGridPosition, AGridUnit*> LocationGridUnitMap;
	UPROPERTY()
	AGridUnit* GridUnitHovered = nullptr;
	UFUNCTION(BlueprintCallable)
	AGridUnit* GetGridUnitHovered() const { return GridUnitHovered; }
	UPROPERTY()
	AGridUnit* GridUnitSelected = nullptr;
	// AGridUnit* GetGridUnitSelected() const { return GridUnitSelected; }
	
	void UpdateUnitMapping(AGridUnit* GridUnit);
	void UpdateUnitMappingsAll();
	AGridUnit* GetGridUnitOnTile(const AGridTile* GridTile) const;
	AGridTile* GetGridTileOfUnit(const AGridUnit* GridUnit) const;

	// calculations (could set a class that does all this so that we can swap out calculation types)
	TArray<AGridTile*> GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range);
	TArray<AGridTile*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);
	void CalculateGridMovement(TArray<FGridMovement>& OutMovement, const AGridUnit* GridUnit);

	

	///////////////////////////////////////////
	// event system, could we move this a separate world subsystem?
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitAbilityDelegate OnGridUnitAbilityStart;
	UPROPERTY(BlueprintAssignable, Category="Turn Based | Grid")
	FGridUnitAbilityDelegate OnGridUnitAbilityEnd;
	
	TArray<TPair<AActor*, UGameplayAbility*>> GridUnitsTakingActions;
	UFUNCTION()
    void OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility);
    UFUNCTION()
    void OnGridUnitAbilityEnded(UGameplayAbility* InGameplayAbility);
	
};

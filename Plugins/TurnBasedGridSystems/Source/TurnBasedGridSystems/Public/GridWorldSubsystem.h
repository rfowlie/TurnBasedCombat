// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridStructs.h"
#include "GridWorldSubsystem.generated.h"

class UObjectPool;
class AActorObjectPool;
class UGridMapBase;
class AGridInfo;
class AGridUnitBase;
class AGridTileBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridTileDelegate, AGridTileBase*, GridTile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridUnitDelegate, AGridUnitBase*, GridUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridUnitAbilityDelegate);

/**
 * organize, manage and broadcast all things related to the grid
 */
UCLASS(Blueprintable)
class TURNBASEDGRIDSYSTEMS_API UGridWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void PostInitialize() override;
	
	// info
	UFUNCTION(BlueprintCallable)
	void RegisterGridInfo(AGridInfo* InGridInfo);
	
	UFUNCTION(BlueprintCallable)
	void SetTileHoverActive(const bool IsActive) { bCanTileHover = IsActive; }

private:
	UPROPERTY()
	UGridMapBase* GridMap;

	UPROPERTY()
	bool bCanTileHover = false;
	
	UPROPERTY()
	AGridInfo* GridInfo;

public:
	// tiles
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

	UFUNCTION(BlueprintCallable)
	void RegisterGridTile(AGridTileBase* GridTile);

private:
	UFUNCTION()
	void OnBeginCursorOverGridTile(AActor* Actor);

	// TODO: encapsulate this into a class
	void UpdateTileMapping(AGridTileBase* GridTile);
	void UpdateTileMappingsAll();

public:
	UPROPERTY()
	TArray<AGridTileBase*> GridTilesAll;
	UPROPERTY()
	TMap<AGridTileBase*, FGridPosition> GridTileLocationMap;
	UPROPERTY()
	TMap<FGridPosition, AGridTileBase*> LocationGridTileMap;
	UPROPERTY()
	AGridTileBase* GridTileHovered = nullptr;
	UFUNCTION(BlueprintCallable)
	AGridTileBase* GetGridTileHovered() const { return GridTileHovered; }
	UPROPERTY()
	AGridTileBase* GridTileSelected = nullptr;
	// AGridTile* GetGridTileSelected() const { return GridTileSelected; }
	////////////////////////////////////////

	
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
	
	UFUNCTION(BlueprintCallable)
	void RegisterGridUnit(AGridUnitBase* GridUnit);

private:
	void UpdateUnitMapping(AGridUnitBase* GridUnit);
	void UpdateUnitMappingsAll();

public:
	UPROPERTY()
	TArray<AGridUnitBase*> GridUnitsAll;
	UPROPERTY()
	TMap<AGridUnitBase*, FGridPosition> GridUnitLocationMap;
	UPROPERTY()
	TMap<FGridPosition, AGridUnitBase*> LocationGridUnitMap;
	UPROPERTY()
	AGridUnitBase* GridUnitHovered = nullptr;
	UFUNCTION(BlueprintCallable)
	AGridUnitBase* GetGridUnitHovered() const { return GridUnitHovered; }
	UPROPERTY()
	AGridUnitBase* GridUnitSelected = nullptr;
	// AGridUnit* GetGridUnitSelected() const { return GridUnitSelected; }
	
	//utility
	AGridUnitBase* GetGridUnitOnTile(const AGridTileBase* GridTile) const;
	AGridTileBase* GetGridTileOfUnit(const AGridUnitBase* GridUnit) const;

	// calculations (could set a class that does all this so that we can swap out calculation types)
	TArray<AGridTileBase*> GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range);
	TArray<AGridTileBase*> GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges);
	void CalculateGridMovement(TArray<FGridMovement>& OutMovement, const AGridUnitBase* GridUnit, const int32 AvailableMovement);


	// Cursor ~ start
	UPROPERTY()
	TMap<TSubclassOf<AActor>, UObjectPool*> CursorPools;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetCursorByClass(TSubclassOf<AActor> CursorClass);
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetCursorsByClass(TSubclassOf<AActor> CursorClass, int32 Amount);
	// Cursor ~ end
};

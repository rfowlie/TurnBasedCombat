// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridMapBase.h"
#include "GridSystemsStructs.h"
#include "GridMapper_FlatEqualSpacing.generated.h"


class AGridUnitBase;

/**
 * this grid will function under the assumption that tiles are all on the same level and evenly spaced apart
 */
UCLASS()
class TURNBASEDGRIDSYSTEMS_API UGridMapper_FlatEqualSpacing : public UGridMapBase
{
	GENERATED_BODY()

	/*
	 * what are the core questions we need to be able to ask a grid mapper?
	 * request adjacent tiles to a passed in tile
	 * NOT movement calculation, we would want that in another class that is more custom and intune
	 * with the games design and special features
	 * Just the abstract visual map of the grid...
	 * 
	 */

	
	// tile
public:
	virtual void RegisterGridTile(AGridTileBase* InGridTile) override;
	virtual void RegisterGridTiles(const TArray<AGridTileBase*>& InGridTiles) override;
	
	UFUNCTION(BlueprintCallable)
	AGridTileBase* GetGridTileOfUnit(const AGridUnitBase* GridUnit) const;

protected:
	UPROPERTY()
	TMap<AGridTileBase*, FGridPosition> GridTileLocationMap;
	
	UPROPERTY()
	TMap<FGridPosition, AGridTileBase*> LocationGridTileMap;

	// unit
public:
	UFUNCTION(BlueprintCallable)
	void RegisterGridUnit(AGridUnitBase* InGridUnit);

	UFUNCTION(BlueprintCallable)
	AGridUnitBase* GetGridUnitOnTile(const AGridTileBase* GridTile) const;
	
protected:
	UPROPERTY()
	TMap<AGridUnitBase*, FGridPosition> GridUnitLocationMap;
	
	UPROPERTY()
	TMap<FGridPosition, AGridUnitBase*> LocationGridUnitMap;

	// utility
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GridSize = 200;
	
	UFUNCTION(BlueprintCallable)
	FGridPosition CalculateGridPosition(const AActor* Actor);
};

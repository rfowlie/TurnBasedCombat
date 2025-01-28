// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MoveAbilityPayload.generated.h"

class AGridTile;

/**
 * 
 */
UCLASS(BlueprintType)
class TURNBASEDCOMBAT_API UMoveAbilityPayload : public UObject
{
	GENERATED_BODY()

	UMoveAbilityPayload();
	
public:
	static UMoveAbilityPayload* Create(const TArray<AGridTile*>& InTiles)
	{
		UMoveAbilityPayload* MoveAbilityPayload = NewObject<UMoveAbilityPayload>();
		MoveAbilityPayload->Tiles = InTiles;
		return MoveAbilityPayload;
	}
	
	UFUNCTION(BlueprintCallable)
	AGridTile* GetNextTile()
	{
		if (CurrentTileIndex == Tiles.Num()) { return nullptr; }
		CurrentTileIndex++;
		return Tiles[CurrentTileIndex];
	}

protected:
	UPROPERTY(BlueprintReadOnly, Category = "MoveAbility")
	TArray<AGridTile*> Tiles;
	
	int32 CurrentTileIndex = -1;	
};

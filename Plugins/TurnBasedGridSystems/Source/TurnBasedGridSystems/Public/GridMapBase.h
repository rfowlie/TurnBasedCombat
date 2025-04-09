// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridMapBase.generated.h"


class AGridTileBase;
/*
 * base class for grid mappers, given all the tiles decide all the relationships
 * gives coordinates, determines adjacency, etc.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDGRIDSYSTEMS_API UGridMapBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void RegisterGridTile(AGridTileBase* InGridTile);

	UFUNCTION()
	virtual void RegisterGridTiles(const TArray<AGridTileBase*>& InGridTiles);	
	
};

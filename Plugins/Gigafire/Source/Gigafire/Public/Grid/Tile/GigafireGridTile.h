// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile/GridTileBase.h"
#include "GigafireGridTile.generated.h"


UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API AGigafireGridTile : public AGridTileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGigafireGridTile();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GridTile")
	bool IsDeploymentTile();
	
	// TODO: was brainstorming having a controller class be responsible for setting hovered
	// tiles will always need to be able to set on hovered
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);
	
};

USTRUCT(BlueprintType)
struct FGigafireGridTileArray
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AGigafireGridTile*> GridTiles;
	
};
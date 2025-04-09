// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GridStateData.generated.h"

class AGridUnitBase;
class AGridTileBase;


USTRUCT(BlueprintType)
struct TURNBASEDGRIDSYSTEMS_API FGridStatePayload
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GridState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridTileBase* Tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridUnitBase* Unit;
};


USTRUCT(BlueprintType)
struct FGridStateChangeHandlerResults
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bSuccess;
	
	UPROPERTY(BlueprintReadWrite)
	bool bBlockTile;

	UPROPERTY(BlueprintReadWrite)
	bool bBlockUnit;
	
};

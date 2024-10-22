// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainDataAsset.generated.h"


class UMaterialInterface;

UENUM(BlueprintType)
enum class ETileTerrain : uint8
{
	Grass,
	Plain,
	Rock,
	Forest
};

USTRUCT(Blueprintable, BlueprintType)
struct FTerrainStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETileTerrain Terrain = ETileTerrain::Grass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MovementCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DefencePhysical = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DefenceMagical = 1;
};

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UTerrainDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* UIMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTerrainStats TerrainStats;
	
};

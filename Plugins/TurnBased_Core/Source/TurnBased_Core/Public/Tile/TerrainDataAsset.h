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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MovementCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DefencePhysical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DefenceMagical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Avoid;
};

USTRUCT(Blueprintable, BlueprintType)
struct FTileStatsSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTerrainStats TerrainStats;
	
};

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UTerrainDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* UIMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTerrainStats TerrainStats;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TerrainDataAsset.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"


class UTerrainDataAsset;

UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API AGridTile : public AActor
{
	GENERATED_BODY()

public:
	AGridTile();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetState(const FGameplayTag State);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetPlacementLocation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMovementCost() const;
	
	/////////////////////////////////////////////////////////////
	// TEMP VARIABLES FOR EASE OF LOGIC
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTerrainDataAsset* TerrainDataAsset =  nullptr;
	
	FTileStatsSnapshot GetSnapshot() const;
	/////////////////////////////////////////////////////////////

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector PlacementLocation = FVector::ZeroVector;
};

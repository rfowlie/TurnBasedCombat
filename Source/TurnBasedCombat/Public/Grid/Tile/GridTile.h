// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "TurnBasedCombat/Public/Grid/GridStructs.h"
#include "TurnBasedCombat/Public/Stats/Terrain/TerrainDataAsset.h"
#include "GridTile.generated.h"


UCLASS()
class TURNBASEDCOMBAT_API AGridTile : public AActor
{
	GENERATED_BODY()

public:
	AGridTile();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetState(const FGameplayTag State);

	/////////////////////////////////////////////////////////////
	// TEMP VARIABLES FOR EASE OF LOGIC
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTerrainDataAsset* TerrainDataAsset =  nullptr;
	
	FTileStatsSnapshot GetSnapshot() const;
	/////////////////////////////////////////////////////////////

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	
};

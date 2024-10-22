// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/Tile/GridTile.h"


AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGridTile::SetState_Implementation(const FGameplayTag State)
{
	return false;
}

FTileStatsSnapshot AGridTile::GetSnapshot() const
{
	FTileStatsSnapshot Value;
	// Value.TerrainStats.Terrain = TerrainType;
	// Value.TerrainStats.MovementCost = MovementCost;
	return Value;
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();	
}

void AGridTile::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
}

void AGridTile::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GridTile.h"

#include "Tile/GridTileUtility.h"
#include "Tile/TerrainDataAsset.h"


AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = false;
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

FVector AGridTile::GetPlacementLocation_Implementation() const
{
	return GetActorLocation() + PlacementLocation;
}

int32 AGridTile::GetMovementCost_Implementation() const
{
	if (!TerrainDataAsset)
	{
		// UE_LOG(LogTemp, Warning, TEXT("No Terrain Data Asset Assigned To Tile: %s, movement cost = 1"), *this->GetName());
		return 1;
	}
	return TerrainDataAsset->TerrainStats.MovementCost;
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

	UGridTileUtility::RegisterGridTile(this);
	
}

void AGridTile::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
}

void AGridTile::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
}




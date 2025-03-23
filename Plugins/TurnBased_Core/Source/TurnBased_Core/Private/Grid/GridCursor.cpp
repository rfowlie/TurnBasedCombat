// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridCursor.h"
#include "Tile/GridTile.h"


AGridCursor::AGridCursor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridCursor"));
}

void AGridCursor::ShowTileCursor(bool bShow)
{
	TileCursorVisible = bShow;
	SetActorHiddenInGame(!bShow);
}

void AGridCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGridCursor::CreateCursor()
{
}

void AGridCursor::UpdateCursor(AGridTile* GridTile)
{
	// only update if cursor visible
	if (TileCursorVisible)
	{
		SetActorLocation(GridTile->GetActorLocation() + HeightAboveTile);
	}	
}

void AGridCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


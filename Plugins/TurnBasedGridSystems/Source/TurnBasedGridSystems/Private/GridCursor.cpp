// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCursor.h"
#include "GridTileBase.h"



void AGridCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

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



void AGridCursor::CreateCursor()
{
}

void AGridCursor::UpdateCursor(AGridTileBase* InGridTile)
{
	// only update if cursor visible
	if (TileCursorVisible)
	{
		SetActorLocation(InGridTile->GetActorLocation() + HeightAboveTile);
	}
}

void AGridCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


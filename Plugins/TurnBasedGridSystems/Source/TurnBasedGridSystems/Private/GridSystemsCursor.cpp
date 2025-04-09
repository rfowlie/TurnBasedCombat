// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystemsCursor.h"
#include "GridTileBase.h"


void AGridSystemsCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

AGridSystemsCursor::AGridSystemsCursor()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridCursor"));
}

void AGridSystemsCursor::ShowTileCursor(bool bShow)
{
	TileCursorVisible = bShow;
	SetActorHiddenInGame(!bShow);
}

void AGridSystemsCursor::CreateCursor()
{
}

void AGridSystemsCursor::UpdateCursor(AGridTileBase* InGridTile)
{
	// only update if cursor visible
	if (TileCursorVisible)
	{
		SetActorLocation(InGridTile->GetActorLocation() + HeightAboveTile);
	}
}

void AGridSystemsCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


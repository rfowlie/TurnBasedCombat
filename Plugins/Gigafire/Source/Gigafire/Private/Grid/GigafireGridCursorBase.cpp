// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GigafireGridCursorBase.h"
#include "Subsystem/GridTrackerSubsystem.h"
#include "Tile/GridTileBase.h"


// Sets default values
AGigafireGridCursorBase::AGigafireGridCursorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

}

// Called when the game starts or when spawned
void AGigafireGridCursorBase::BeginPlay()
{
	Super::BeginPlay();

	if (UGridTrackerSubsystem* Subsystem = GetWorld()->GetSubsystem<UGridTrackerSubsystem>())
	{
		Subsystem->OnGridTileHoveredStart.AddUniqueDynamic(this, &ThisClass::UpdateCursor);
	}
}

void AGigafireGridCursorBase::CreateCursor()
{
	StaticMeshComponent->SetStaticMesh(CursorMesh);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetAffectDistanceFieldLighting(false);
	StaticMeshComponent->SetAffectDynamicIndirectLighting(false);
	TileCursorScale = TileCursorScale.GetAbs();
	StaticMeshComponent->SetWorldScale3D(TileCursorScale);
	
	EnableCursor(false);
}

void AGigafireGridCursorBase::EnableCursor(bool bValue)
{
	IsEnabled = bValue;
	SetActorHiddenInGame(!bValue);
}

void AGigafireGridCursorBase::PauseCursor(bool bValue)
{
	IsEnabled = bValue;
}

void AGigafireGridCursorBase::UpdateCursor(AGridTileBase* GridTile)
{
	if (IsEnabled)
	{
		SetActorLocation(GridTile->GetActorLocation() + Cursor_ExtraHeight);
		UE_LOG(LogTemp, Display, TEXT("Cursor Updated"));
	}	
}



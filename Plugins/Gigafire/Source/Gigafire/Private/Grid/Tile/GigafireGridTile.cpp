// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Tile/GigafireGridTile.h"


// Sets default values
AGigafireGridTile::AGigafireGridTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGigafireGridTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGigafireGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


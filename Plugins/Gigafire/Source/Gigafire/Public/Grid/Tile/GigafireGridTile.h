// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GigafireGridTile.generated.h"


UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API AGigafireGridTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGigafireGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridTileBeginCursorOver, AActor*, GridTile );

	// allow for grid actors to customize what triggers the cursor over
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGridTileBeginCursorOver OnGridTileBeginCursorOver;

	// TODO: was brainstorming having a controller class be responsible for setting hovered
	// tiles will always need to be able to set on hovered
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);
};



USTRUCT(BlueprintType)
struct FGigaGridTileArray
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AGigafireGridTile*> GridTiles;
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTileBase.generated.h"

struct FGridStatePayload;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FGridTileBeginCursorOver, AGridTileBase*, GridTile );

UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDGRIDSYSTEMS_API AGridTileBase : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	// generic cursor over that can be called in a way that suits design and art
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGridTileBeginCursorOver OnGridTileBeginCursorOver;

	// allow each tile to control how it updates it's state according to the game
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGridState(const FGridStatePayload Payload);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	int32 GetMovementCost();
	
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	// FVector GetPlacementLocation() const;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// FVector PlacementLocation = FVector::ZeroVector;
};

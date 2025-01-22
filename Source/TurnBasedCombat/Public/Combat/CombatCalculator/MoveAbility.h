// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MoveAbility.generated.h"


class AGridTile;
class ULerpMovementHelper;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveFinished, AActor*, MovedActor);

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UMoveAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:	
	// Function to set movement locations
	UFUNCTION(BlueprintCallable, Category = "MoveAbility")
	void SetMovementLocations(const TArray<FVector>& NewLocations);
	
	UFUNCTION(BlueprintCallable, Category = "MoveAbility")
	void SetMovementTiles(TArray<AGridTile*> InTiles) { Tiles = InTiles; }

	UPROPERTY(BlueprintReadOnly, Category = "MoveAbility")
	TArray<AGridTile*> Tiles;
	
	// Delegate to notify when movement finishes
	UPROPERTY(BlueprintAssignable, Category = "MoveAbility")
	FOnMoveFinished OnMoveFinished;

	// Locations to move through
	UPROPERTY(BlueprintReadOnly, Category = "MoveAbility")
	TArray<FVector> MovementLocations;
	
protected:

	// Current index in the movement sequence
	int32 CurrentLocationIndex;

	int32 CurrentTileIndex = -1;
	UFUNCTION(BlueprintCallable)
	AGridTile* GetNextTile();

	UFUNCTION(BlueprintCallable)
	FVector GetNextLocation();

	// Function to start movement to the next location
	void MoveToNextLocation();

	// Callback for when movement to a location finishes
	UFUNCTION()
	void OnMovementCompleted();

	float GetLerpDuration() const { return 1.0f; }
	
	UPROPERTY()
	ULerpMovementHelper* LerpHelper;
};

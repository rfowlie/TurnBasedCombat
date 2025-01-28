// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/MoveAbility.h"

#include "Combat/CombatCalculator/LerpMovementHelper.h"


void UMoveAbility::SetMovementLocations(const TArray<FVector>& NewLocations)
{
	MovementLocations = NewLocations;
	CurrentLocationIndex = 0;
}

AGridTile* UMoveAbility::GetNextTile()
{
	if (CurrentTileIndex == Tiles.Num()) { return nullptr; }
	CurrentTileIndex++;
	return Tiles[CurrentTileIndex];
}

FVector UMoveAbility::GetNextLocation()
{
	if (CurrentLocationIndex < MovementLocations.Num())
	{
		CurrentLocationIndex++;
		return MovementLocations[CurrentLocationIndex];
	}

	return FVector();
}

void UMoveAbility::MoveToNextLocation()
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (!LerpHelper)
		{
			LerpHelper = NewObject<ULerpMovementHelper>(this);
		}

		LerpHelper->OnLerpFinished.AddDynamic(this, &UMoveAbility::OnMovementCompleted);
		LerpHelper->StartLerp(Actor, Actor->GetActorLocation(), MovementLocations[CurrentLocationIndex], GetLerpDuration()); // Adjust duration as needed
	}
}

void UMoveAbility::OnMovementCompleted()
{
	if (CurrentLocationIndex < MovementLocations.Num())
	{
		++CurrentLocationIndex;
		MoveToNextLocation();
	}
	else
	{
		// Notify when movement finishes
		OnMoveFinished.Broadcast(GetAvatarActorFromActorInfo());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}	
}
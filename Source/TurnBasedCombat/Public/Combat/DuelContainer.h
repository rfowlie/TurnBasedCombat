// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DuelContainer.generated.h"


class AGridTile;
class AGridUnit;

/*
 *	just like grid proxy, this will be a wrapper class that we can use to answer combat related
 *	questions and encapsulate complicated behaviour from UI, etc.
 *	Simulate combat???
 */
UCLASS(BlueprintType)
class TURNBASEDCOMBAT_API UDuelContainer : public UObject
{
	GENERATED_BODY()

	UDuelContainer();

public:
	static UDuelContainer* CreateContainer(AGridUnit* InInstigatorUnit, AGridTile* InInstigatorTile,
		AGridUnit* InTargetUnit, AGridTile* InTargetTile);

	UPROPERTY()
	AGridUnit* InstigatorUnit;
	UPROPERTY()
	AGridTile* InstigatorTile;
	UPROPERTY()
	AGridUnit* TargetUnit;
	UPROPERTY()
	AGridTile* TargetTile;
	
	UFUNCTION(BlueprintCallable)
	int32 GetInstigatorDamage() const;
	UFUNCTION(BlueprintCallable)
	int32 GetInstigatorHitPercentage() const;

	UFUNCTION(BlueprintCallable)
	int32 GetTargetDamage() const;
	UFUNCTION(BlueprintCallable)
	int32 GetTargetHitPercentage() const;
	
};

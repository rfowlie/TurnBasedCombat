// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "DuelContainer.generated.h"


class AGridTile;
class AGridUnit;


// define all the calculations we would like to expose externally (BLUEPRINT, UI, etc.)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Duel_Instigator_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Duel_Instigator_Accuracy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Duel_Target_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Duel_Target_Accuracy);

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

	// for UI, so that it can query something cleanly without issue...
	UFUNCTION(BlueprintCallable)
	FString GetDuelAttribute(FGameplayTag InTag) const;

protected:
	UPROPERTY()
	AGridUnit* InstigatorUnit;
	UPROPERTY()
	AGridTile* InstigatorTile;
	UPROPERTY()
	AGridUnit* TargetUnit;
	UPROPERTY()
	AGridTile* TargetTile;
	
	
	int32 GetInstigatorDamage() const;
	int32 GetInstigatorHitPercentage() const;
	int32 GetTargetDamage() const;
	int32 GetTargetHitPercentage() const;
	
};

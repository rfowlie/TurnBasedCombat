// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatDisplayWidget.generated.h"

class AGridUnit;
class UWeaponDataAsset;

USTRUCT(Blueprintable, BlueprintType)
struct FCombatDisplayInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AGridUnit* Instigator;

	UPROPERTY(BlueprintReadOnly)
	AGridUnit* Target;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UWeaponDataAsset*> ValidWeapons;
};

/**
 * 
 */
UCLASS(Blueprintable)
class TURNBASEDCOMBAT_API UCombatDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCombatInfo(FCombatDisplayInfo CombatDisplayInfo);

protected:
	UPROPERTY()
	FCombatDisplayInfo CurrentCombatDisplayInfo;
};

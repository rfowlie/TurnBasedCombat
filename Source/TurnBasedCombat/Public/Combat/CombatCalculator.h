// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CombatCalculator.generated.h"

/**
 * Combat calculator will be an abstract class that wraps all the required objects and information
 * for determining outcomes in combat
 * ??? should this be a container? that focuses on only one interaction to simplify
 */

// TODO: make abstract one the design is a little more planned out
UCLASS()
class TURNBASEDCOMBAT_API UCombatCalculator : public UObject
{
	GENERATED_BODY()

public:
	FString GetCombatAttribute(FGameplayTag InTag);
};

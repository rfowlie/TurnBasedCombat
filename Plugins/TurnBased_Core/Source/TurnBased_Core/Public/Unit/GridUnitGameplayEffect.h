// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "GridUnitGameplayEffect.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Set_Unit_Health);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Set_Unit_Movement);

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UGridUnitGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGridUnitGameplayEffect();
};

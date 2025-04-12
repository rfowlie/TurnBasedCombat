// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GridUnitAttributeSet.h"
#include "GridUnit_Interface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "GridUnit.generated.h"


class UGridUnitBehaviourComponent;
class UActionBehaviour_Combat;
class UWeaponDataAsset;
class UMoveAbility;
class UAbilityAsync_WaitAttributeChanged;
class UGridUnitAttributeSet;
class UGameplayAbility;
class UAttributeSet;
class UStatsDataAsset;
class UWeaponDataAsset_OLD;
class UWeapon;
class UItemBase;


DECLARE_DELEGATE(FAnimCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationDelegate, AGridUnit*, GridUnit);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridUnitAbilityDelegate);
DECLARE_DYNAMIC_DELEGATE(FUseAbilityDelegate);
DECLARE_DELEGATE_OneParam(FGameplayAbilityEndCallback, UGameplayAbility*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridUnitEventDelegate, AGridUnit*, GridUnit);


UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API AGridUnit : public AActor, public IAbilitySystemInterface, public IGridUnit_Interface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	AGridUnit();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetState(const FGameplayTag& State);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsAlive() const;
	
};

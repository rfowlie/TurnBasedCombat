// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "Unit/GridUnit.h"
#include "Unit/GridUnitAttributeSet.h"
#include "Unit/GridUnit_Interface.h"
#include "_FireEmblemLike/Grid/Maps/GridUnit_FlatNoHeight.h"
#include "FireEmblemUnit.generated.h"

class UAbilityAsync_WaitAttributeChanged;
class UGridUnitBehaviourComponent;
class UGridUnitAttributeSet;

// DECLARED IN AGRIDUNIT
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridUnitEventDelegate, AFireEmblemUnit*, GridUnit);

UCLASS(Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API AFireEmblemUnit : public AGridUnit, public IGridUnit_FlatNoHeight
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	AFireEmblemUnit();
	virtual void Tick(float DeltaTime) override;
	
	// Ability System ~ start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGridUnitAttributeSet* AttributeSet_GridUnit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass_Move;	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle_Move;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass_Attack;
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle_Attack;
	
	UFUNCTION(BlueprintCallable)
	int32 GetHealth() const { return AttributeSet_GridUnit->GetHealth(); }
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnitDefeated();
	UPROPERTY()
	UAbilityAsync_WaitAttributeChanged* WaitForHealthZero;
	UFUNCTION()
	void NotifyHealthZero();
	// Ability System ~ end

public:
	// Stats ~ start
	UPROPERTY(EditInstanceOnly)
	UStatsDataAsset* StatsDataAsset;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99, UIMin=1, UIMax=99))
	int32 Level = 1;
	// Stats ~ end

	// Animation ~ start
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* MovementStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* MovementEnd;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Attack;
	// Animation ~ end

	// Weapon ~ start
	// DEPRECATED - do not delete for now, until we can find any useful things in the project that use it
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (Categories = "Weapon.Type"))
	FGameplayTagContainer WeaponsInventory;
	FGameplayTag EquippedWeapon;
	UFUNCTION(BlueprintCallable)
	void SetEquippedWeapon(FGameplayTag WeaponToEquip);
	UFUNCTION(BlueprintCallable)
	virtual FGameplayTag GetEquippedWeapon();
	TSet<int32> GetWeaponRanges() const;

	// TODO: WHAT IS THE BEST WAY TO GO ABOUT THISSSS
	// weapon - dataasset
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UWeaponDataAsset*> Weapons;
	UPROPERTY(BlueprintReadOnly)
	UWeaponDataAsset* WeaponEquipped;
	UFUNCTION(BlueprintCallable)
	void SetWeaponEquipped(UWeaponDataAsset* InWeaponDataAsset);
	UFUNCTION(BlueprintCallable)
	virtual UWeaponDataAsset* GetWeaponEquipped();
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetAllWeaponRanges() const;

	// weapon - FNAME (data table)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta=(GetKeyOptions="GetWeaponNameOptions"))
	TMap<FName, bool> WeaponInventoryMap;
	// TODO: currently could have a different equipped weapon than in map, might cause problems
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta=(GetKeyOptions="GetWeaponNameOptions"))
	FName EquippedWeaponName;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<FName> GetWeaponNameOptions();

	UFUNCTION(BlueprintCallable)
	void SetEquippedWeaponName(const FName InName) { if (WeaponInventoryMap.Contains(InName)) { EquippedWeaponName = InName; }}

	UFUNCTION(BlueprintCallable)
	FName GetEquippedWeaponName() const { return EquippedWeaponName; }

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetWeaponsInMap() const;
	// Weapon ~ end
	
	// Action Behaviour ~ start
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGridUnitBehaviourComponent* CombatBehaviourComponent = nullptr;
	
	// Action Behaviour ~ end

};

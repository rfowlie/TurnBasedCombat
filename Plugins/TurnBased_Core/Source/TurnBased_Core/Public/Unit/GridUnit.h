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

	// Ability System ~ start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGridUnitAttributeSet* AttributeSet_GridUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
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
	UPROPERTY(BlueprintAssignable)
	FGridUnitEventDelegate OnDefeat;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void EventOnDefeat();
	UPROPERTY()
	UAbilityAsync_WaitAttributeChanged* WaitForHealthZero;
	UFUNCTION()
	void NotifyHealthZero();
	UFUNCTION()
	void OnHealthZero(FGameplayAttribute Attribute, float NewValue, float OldValue)
	{
		UE_LOG(LogTemp, Log, TEXT("On Health Zero"));
		if (NewValue == 0)
		{
			EventOnDefeat();
			if (OnDefeat.IsBound()) { OnDefeat.Broadcast(this); }
		}		
	}
	// Ability System ~ end

public:
	// Placeholders ~ start
	// TODO: temp placeholder variables
	// will remove when we switch to calling abilities through gameplay events passing in event data
	UPROPERTY(BlueprintReadOnly)
	FVector MoveAbilityLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly)
	AGridUnit* AttackAbilityTarget = nullptr;
	// Placeholders ~ end
	
	// DECLARE_EVENT(AGridUnit, FGridUnitAbilityEvent)
	// FGridUnitAbilityEvent OnAbilityMoveEnd;
	// FGridUnitAbilityEvent OnAbilityAttackEnd;
	FGridUnitEventDelegate OnEventMoveEnd;
	FGridUnitEventDelegate OnEventAttackEnd;
	
	UPROPERTY(EditInstanceOnly)
	UStatsDataAsset* StatsDataAsset;	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetState(const FGameplayTag& State);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99, UIMin=1, UIMax=99))
	int32 Level = 1;	
	
	UFUNCTION(BlueprintCallable)
	virtual int32 GetAvailableMovement() const;

	// temp animation shit
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAnimationDelegate AnimationPre;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAnimationDelegate AnimationPost;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAnimationDelegate AnimationDuring;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* MovementStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* MovementEnd;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Attack;
	
	bool MovementEvent(const FVector& Location);
	bool AttackEvent(const FVector& Location, AGridUnit* Target);

	// Just for setup, will then need to instance to keep track of changes
	// UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	// TArray<UWeaponDataAsset*> WeaponDataAssets;	

public:
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

private:
	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& Data);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "TurnBasedCombat/Public/Stats/StatsDataAsset.h"
#include "GridUnit.generated.h"


class UGameplayAbility;
class UAttributeSet;
class UWeaponDataAsset;
class UWeapon;
class UItemBase;


DECLARE_DELEGATE(FAnimCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationDelegate, AGridUnit*, GridUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridUnitAbilityDelegate);
DECLARE_DYNAMIC_DELEGATE(FUseAbilityDelegate);
DECLARE_DELEGATE_OneParam(FGameplayAbilityEndCallback, UGameplayAbility*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridUnitEventDelegate, AGridUnit*, GridUnit);


UCLASS(Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API AGridUnit : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	AGridUnit();
	virtual void Tick(float DeltaTime) override;

	// Ability System - start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass_Move;	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle_Move;

	UPROPERTY(BlueprintReadOnly)
	FVector MoveAbilityLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly)
	AGridUnit* AttackAbilityTarget = nullptr;
	
	// DECLARE_EVENT(AGridUnit, FGridUnitAbilityEvent)
	// FGridUnitAbilityEvent OnAbilityMoveEnd;
	// FGridUnitAbilityEvent OnAbilityAttackEnd;
	FGridUnitEventDelegate OnEventMoveEnd;
	FGridUnitEventDelegate OnEventAttackEnd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass_Attack;	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle_Attack;
	// Ability System - end
	
	UPROPERTY(EditInstanceOnly)
	UStatsDataAsset* StatsDataAsset;	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetState(FGameplayTag State);
	
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
	
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool MovementEvent(const FVector& Location);
	bool AttackEvent(const FVector& Location, AGridUnit* Target);

protected:
	// Just for setup, will then need to instance to keep track of changes
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UWeaponDataAsset*> WeaponDataAssets;

public:
	UFUNCTION(BlueprintCallable)
	FName GetFaction() const;

	TSet<int32> GetWeaponRanges() const;

	// UNIT STATS start
protected:	
	UPROPERTY()
	FUnitStatsSnapshot UnitStatsSnapshot;
	
public:		
	UFUNCTION(BlueprintCallable)
	FUnitStatsSnapshot GetSnapshot() const;
	
	UFUNCTION()
	void UpdateStats(const FUnitStatsSnapshot& StatAdjustments);
	
	// UNIT STATS end

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Faction;

	//////////////////////////////////////////////////////
	///

private:
	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& Data);
};

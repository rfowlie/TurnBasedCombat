// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnBasedCombat/Public/Stats/StatsDataAsset.h"
#include "GridUnit.generated.h"


class UWeaponDataAsset;
class UWeapon;
class UItemBase;


DECLARE_DELEGATE(FAnimCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationDelegate, AGridUnit*, GridUnit);

UCLASS(Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API AGridUnit : public AActor
{
	GENERATED_BODY()

public:
	AGridUnit();
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetHovered(bool IsHovered);

	UPROPERTY(EditInstanceOnly)
	UStatsDataAsset* StatsDataAsset;
	
protected:
	virtual void BeginPlay() override;


public:
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
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AnimationMovement();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AnimationAttack();

private:
	UPROPERTY()
	TArray<UWeapon*> EquippedWeapons;
	
	UPROPERTY()
	TArray<UItemBase*> EquippedItems;

protected:
	// Just for setup, will then need to instance to keep track of changes
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UWeaponDataAsset*> WeaponDataAssets;

public:
	UFUNCTION(BlueprintCallable)
	FName GetFaction() const;
	
	UFUNCTION(BlueprintCallable)
	TArray<UWeapon*> GetEquippedWeapons() const;

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
	int32 Team;
};

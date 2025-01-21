// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "WeaponSolver.generated.h"

class UGridManager;
class AGridUnit;
class UStatSolver;
struct FCombatCalculatorPayload_FireEmblem;


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Might);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Weight);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Hit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Range);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Trait_Special_Light);


USTRUCT(BlueprintType)
struct FWeaponSolverStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UStatSolver> StatSolver = nullptr;
};

// USTRUCT(BlueprintType)
// struct FUniqueTrait
// {
// 	GENERATED_BODY()
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FGameplayTag Trait;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TSubclassOf<UStatSolver> StatSolver = nullptr;
// };

USTRUCT(BlueprintType)
struct FWeaponTraits : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Might = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Range = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalBonus = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer SpecialTraits;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TArray<FUniqueTrait> UniqueTraits;
	
};
/**
 * For now do not worry about making generic
 * implement the logic for unique traits here, force BPs to implement get weapon
 * as it will most likely be easier to make a DT for them...
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UWeaponSolver : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void GetWeaponTraits(FWeaponTraits& WeaponTraits, const FGameplayTag InWeapon) const;
	
	// UFUNCTION(BlueprintCallable)
	// virtual int32 GetWeaponTrait(const FGameplayTag InWeapon, const FGameplayTag InTrait) const;

	virtual void Solve(
		TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
		const FCombatCalculatorPayload_FireEmblem& Payload);
	virtual void Solve(
		TMap<FGameplayTag, int32>& Attributes, TArray<FGameplayTag> Traits,
		const UGridManager* GridManager, const AGridUnit* Instigator, const AGridUnit* Target);
	
	
	void SetGridManager(UGridManager* InGridManager) { GridManager = InGridManager; }
	void SetInstigator(AGridUnit* InInstigator) { Instigator = InInstigator; }
	void SetTarget(AGridUnit* InTarget) { Target = InTarget; }
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void GetWeapon(FWeaponTraits& WeaponTraits, FGameplayTag Weapon) const;

	// FOR NOW... just do here, can eventually use a DT or allow for custom setup
	UFUNCTION(BlueprintImplementableEvent)
	void AddSpecialTrait(FWeaponTraits& WeaponTraits, FGameplayTag SpecialTrait) const;
	
	UPROPERTY()
	UGridManager* GridManager;
	UPROPERTY()
	AGridUnit* Instigator;
	UPROPERTY()
	AGridUnit* Target;
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "_FireEmblemLike/Unit/FireEmblemUnit.h"
#include "Unit/Components/GridUnitBehaviourComponent.h"
#include "Combat/Weapon/WeaponDataAsset.h"


AFireEmblemUnit::AFireEmblemUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet_GridUnit = CreateDefaultSubobject<UGridUnitAttributeSet>(TEXT("AttributeSet_GridUnit"));
	CombatBehaviourComponent = CreateDefaultSubobject<UGridUnitBehaviourComponent>(TEXT("CombatBehaviourComponent"));
}

void AFireEmblemUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireEmblemUnit::BeginPlay()
{
	Super::BeginPlay();

	// TODO: reactivate this line once inheriting properly
	// UGridUnitUtility::RegisterGridUnit(this);
	
	// register with grid manager
	// if (ATurnBasedCombatGameMode* GameMode = Cast<ATurnBasedCombatGameMode>(GetWorld()->GetAuthGameMode()))
	// {
	// 	GameMode->RegisterGridUnit(this);
	// }
	
	// FGameplayAbilitySpec Spec = FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this);
	// Spec.Ability->OnGameplayAbilityEnded.AddLambda([](UGameplayAbility* Ability)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("I don't understand..."));
	// });

	// give attribute set
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet_GridUnit);
	// assign stats
	if (!IsValid(StatsDataAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("No Stats Data Asset Assigned To Unit: %s"), *this->GetName());
	}
	else
	{
		AttributeSet_GridUnit->InitializeAttributesFromStatsDataAsset(StatsDataAsset, Level);
	}

	// follow health attribute, when hits zero then do defeat stuff (anim, broadcast, etc.)
	// WaitForHealthZero = UAbilityAsync_WaitAttributeChanged::WaitForAttributeChanged(
	// 	this, UGridUnitAttributeSet::GetHealthAttribute(), false);
	// WaitForHealthZero->Changed.AddDynamic(this, &ThisClass::OnHealthZero);
	AttributeSet_GridUnit->OnHealthZero.AddUObject(this, &ThisClass::NotifyHealthZero);
	
	// give move ability
	GameplayAbilitySpecHandle_Move = AbilitySystemComponent->GiveAbility(
		   FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this));
	
	
	// give attack ability
	GameplayAbilitySpecHandle_Attack = AbilitySystemComponent->GiveAbility(
		   FGameplayAbilitySpec(GameplayAbilityClass_Attack, 1, INDEX_NONE, this));
	
	// weapons
	// SetEquippedWeapon(WeaponsInventory.First());
	// SetWeaponEquipped(Weapons[0]);
	// SetEquippedWeaponName(WeaponNames[0]);
	TArray<FName> Keys;
	WeaponInventoryMap.GetKeys(Keys);
	if (!Keys.IsEmpty()){ SetEquippedWeaponName(Keys[0]); }
}

void AFireEmblemUnit::NotifyHealthZero()
{
	UE_LOG(LogTemp, Error, TEXT("On Health Zero"));
	OnUnitDefeated();
	// if (OnDefeat.IsBound()) { OnDefeat.Broadcast(this); }
}

int32 AFireEmblemUnit::GetAvailableMovement() const
{
	return AttributeSet_GridUnit->GetMovement();
}

void AFireEmblemUnit::SetEquippedWeapon(FGameplayTag WeaponToEquip)
{
	if (WeaponsInventory.HasTagExact(WeaponToEquip))
	{
		EquippedWeapon = WeaponToEquip;
	}
}

FGameplayTag AFireEmblemUnit::GetEquippedWeapon()
{
	return EquippedWeapon;
}

TSet<int32> AFireEmblemUnit::GetWeaponRanges() const
{
	TSet<int32> OutValues;
	// for (const auto DataAsset : WeaponDataAssets)
	// {
	// 	OutValues.Add(DataAsset->WeaponStats.Range);
	// }

	return OutValues;
}

void AFireEmblemUnit::SetWeaponEquipped(UWeaponDataAsset* InWeaponDataAsset)
{
	if (Weapons.Contains(InWeaponDataAsset)) { WeaponEquipped = InWeaponDataAsset; }
}

UWeaponDataAsset* AFireEmblemUnit::GetWeaponEquipped()
{
	return WeaponEquipped;
}

TArray<int32> AFireEmblemUnit::GetAllWeaponRanges() const
{
	TSet<int32> OutValues;
	for (auto Weapon : Weapons)
	{
		OutValues.Add(Weapon->WeaponTraits.Range);
	}
	return OutValues.Array();
}

TArray<FName> AFireEmblemUnit::GetWeaponsInMap() const
{
	TArray<FName> Keys;
	WeaponInventoryMap.GetKeys(Keys);
	return Keys;
}

/*
 * NOTE
 * This is the setup because we cannot figure out how to get OnGameplayAbilityEnded to fucking fire...
 * so instead we create an event for each expected ability, and fire the event when the ability finishes
 */
void AFireEmblemUnit::OnAbilityEnded(const FAbilityEndedData& Data)
{
	// if (Data.AbilitySpecHandle == GameplayAbilitySpecHandle_Move)
	// {
	// 	if (OnAbilityMoveEnd.IsBound()) { OnAbilityMoveEnd.Broadcast(); }		
	// }
	// if (Data.AbilitySpecHandle == GameplayAbilitySpecHandle_Attack)
	// {
	// 	if (OnAbilityAttackEnd.IsBound()) { OnAbilityAttackEnd.Broadcast(); }		
	// }
}

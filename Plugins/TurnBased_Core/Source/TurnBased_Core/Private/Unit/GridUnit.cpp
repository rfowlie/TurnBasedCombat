// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/GridUnit.h"
#include "AbilitySystemComponent.h"
#include "Combat/Weapon/WeaponDataAsset.h"
#include "Unit/GridUnitAttributeSet.h"
#include "Unit/GridUnitUtility.h"
#include "Unit/Components/GridUnitBehaviourComponent.h"



AGridUnit::AGridUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AGridUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridUnit::BeginPlay()
{
	Super::BeginPlay();

	UGridUnitUtility::RegisterGridUnit(this);
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

	// // give attribute set
	// AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet_GridUnit);
	// // assign stats
	// if (!IsValid(StatsDataAsset))
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("No Stats Data Asset Assigned To Unit: %s"), *this->GetName());
	// }
	// else
	// {
	// 	AttributeSet_GridUnit->InitializeAttributesFromStatsDataAsset(StatsDataAsset, Level);
	// }
	//
	// // follow health attribute, when hits zero then do defeat stuff (anim, broadcast, etc.)
	// // WaitForHealthZero = UAbilityAsync_WaitAttributeChanged::WaitForAttributeChanged(
	// // 	this, UGridUnitAttributeSet::GetHealthAttribute(), false);
	// // WaitForHealthZero->Changed.AddDynamic(this, &ThisClass::OnHealthZero);
	// AttributeSet_GridUnit->OnHealthZero.AddUObject(this, &ThisClass::NotifyHealthZero);
	//
	// // give move ability
	// GameplayAbilitySpecHandle_Move = AbilitySystemComponent->GiveAbility(
	// 	   FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this));
	//
	//
	// // give attack ability
	// GameplayAbilitySpecHandle_Attack = AbilitySystemComponent->GiveAbility(
	// 	   FGameplayAbilitySpec(GameplayAbilityClass_Attack, 1, INDEX_NONE, this));
	//
	// /////////////////////////////////////////////////////////////////////
	// // TESTING ability system shit
	// // AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnded);
	// AbilitySystemComponent->OnAbilityEnded.AddLambda([this](const FAbilityEndedData& Data)
	// {
	// 	// if (Data.AbilitySpecHandle == GameplayAbilitySpecHandle_Move)
	// 	// {
	// 	// 	if (OnEventMoveEnd.IsBound()) { OnEventMoveEnd.Broadcast(this); }
	// 	// }
	// 	if (Data.AbilitySpecHandle == GameplayAbilitySpecHandle_Attack)
	// 	{
	// 		if (OnEventAttackEnd.IsBound()) { OnEventAttackEnd.Broadcast(this); }
	// 	}
	// });
	//
	// // weapons
	// // SetEquippedWeapon(WeaponsInventory.First());
	// // SetWeaponEquipped(Weapons[0]);
	// // SetEquippedWeaponName(WeaponNames[0]);
	// TArray<FName> Keys;
	// WeaponInventoryMap.GetKeys(Keys);
	// if (!Keys.IsEmpty()){ SetEquippedWeaponName(Keys[0]); }
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/Unit/GridUnit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Async/AbilityAsync_WaitAttributeChanged.h"
#include "Grid/Unit/GridUnitAttributeSet.h"
#include "TurnBasedCombat/Public/Item/Weapon.h"
#include "_Framework/GameMode/TurnBasedCombatGameMode.h"


AGridUnit::AGridUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet_GridUnit = CreateDefaultSubobject<UGridUnitAttributeSet>(TEXT("AttributeSet_GridUnit"));
}

void AGridUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridUnit::BeginPlay()
{
	Super::BeginPlay();

	// register with grid manager
	if (ATurnBasedCombatGameMode* GameMode = Cast<ATurnBasedCombatGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RegisterGridUnit(this);
	}

	// IS THIS BEING USED???
	FGameplayAbilitySpec Spec = FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this);
	Spec.Ability->OnGameplayAbilityEnded.AddLambda([](UGameplayAbility* Ability)
	{
		UE_LOG(LogTemp, Error, TEXT("I don't understand..."));
	});

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
	GameplayAbilitySpecHandle_Move = AbilitySystemComponent->GiveAbility(Spec);
	// GameplayAbilitySpecHandle_Move = AbilitySystemComponent->GiveAbility(
	// 	   FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this));
	
	
	// give attack ability
	GameplayAbilitySpecHandle_Attack = AbilitySystemComponent->GiveAbility(
		   FGameplayAbilitySpec(GameplayAbilityClass_Attack, 1, INDEX_NONE, this));

	
	/////////////////////////////////////////////////////////////////////
	// TESTING ability system shit
	// AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnded);
	AbilitySystemComponent->OnAbilityEnded.AddLambda([this](const FAbilityEndedData& Data)
	{
		if (Data.AbilitySpecHandle == GameplayAbilitySpecHandle_Move)
		{
			if (OnEventMoveEnd.IsBound()) { OnEventMoveEnd.Broadcast(this); }
		}
		if (Data.AbilitySpecHandle == GameplayAbilitySpecHandle_Attack)
		{
			if (OnEventAttackEnd.IsBound()) { OnEventAttackEnd.Broadcast(this); }
		}
	});
}

void AGridUnit::NotifyHealthZero()
{
	UE_LOG(LogTemp, Log, TEXT("On Health Zero"));
	EventOnDefeat();
	if (OnDefeat.IsBound()) { OnDefeat.Broadcast(this); }
}

int32 AGridUnit::GetAvailableMovement() const
{
	return AttributeSet_GridUnit->GetMovement();
}

bool AGridUnit::MovementEvent(const FVector& Location)
{
	if (!AbilitySystemComponent) { return false; }
	
	FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GameplayAbilitySpecHandle_Move);
	if (GameplayAbilitySpec->Ability == nullptr) { return false; }

	// set location as parameter on ability???
	MoveAbilityLocation = Location;
		
	// bind to ability end

	// // test 1
	// TMulticastDelegate<void(UGameplayAbility*)>::FDelegate Ended;
	// Ended.BindLambda([](UGameplayAbility* GameplayAbility)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Ended Delegate"));
	// });
	// GameplayAbilitySpec->Ability->OnGameplayAbilityEnded.Add(Ended);
	//
	// // test 2
	// TMulticastDelegate<void(const FAbilityEndedData&)>::FDelegate EndedData;
	// EndedData.BindLambda([](const FAbilityEndedData&)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("EndedData Delegate"));
	// });	
	// GameplayAbilitySpec->Ability->OnGameplayAbilityEndedWithData.Add(EndedData);

	// normal
	// GameplayAbilitySpec->Ability->OnGameplayAbilityEnded.Add(Callback);
	// auto GameplayInstances = GameplayAbilitySpec->GetAbilityInstances();
	
	// activate ability
	return AbilitySystemComponent->TryActivateAbility(GameplayAbilitySpecHandle_Move);
}

bool AGridUnit::AttackEvent(const FVector& Location, AGridUnit* Target)
{
	// TODO
	if (!AbilitySystemComponent || Target == nullptr) { return false; }
	
	FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GameplayAbilitySpecHandle_Attack);
	if (GameplayAbilitySpec->Ability == nullptr) { return false; }

	// TODO: will use GAS to populate data and pass the data instead of the actor holding the information
	// pass loaction and target
	MoveAbilityLocation = Location;
	AttackAbilityTarget = Target;
	
	// activate ability
	return AbilitySystemComponent->TryActivateAbility(GameplayAbilitySpecHandle_Attack);
}

FName AGridUnit::GetFaction() const
{
	// TODO: for now...
	// const FString F = "Faction_" + FString::FromInt(Faction);
	// return "Faction_" + FString::FromInt(Faction);
	return FName(FString::FromInt(Faction));
}

TSet<int32> AGridUnit::GetWeaponRanges() const
{
	TSet<int32> OutValues;
	for (const auto DataAsset : WeaponDataAssets)
	{
		OutValues.Add(DataAsset->WeaponStats.Range);
	}

	return OutValues;
}

/*
 * NOTE
 * This is the setup because we cannot figure out how to get OnGameplayAbilityEnded to fucking fire...
 * so instead we create an event for each expected ability, and fire the event when the ability finishes
 */
void AGridUnit::OnAbilityEnded(const FAbilityEndedData& Data)
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

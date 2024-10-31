// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedCombat/Public/Grid/Unit/GridUnit.h"

#include "AbilitySystemComponent.h"
#include "Grid/Unit/GridUnitAttributeSet.h"
#include "TurnBasedCombat/Public/Item/Weapon.h"
#include "_Framework/GameMode/TurnBasedCombatGameMode.h"


AGridUnit::AGridUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGridUnitAttributeSet>(TEXT("AttributeSet"));
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

	FGameplayAbilitySpec Spec = FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this);
	Spec.Ability->OnGameplayAbilityEnded.AddLambda([](UGameplayAbility* Ability)
	{
		UE_LOG(LogTemp, Error, TEXT("I don't understand..."));
	});
	
	// give move ability
	GameplayAbilitySpecHandle_Move = AbilitySystemComponent->GiveAbility(Spec);
	// GameplayAbilitySpecHandle_Move = AbilitySystemComponent->GiveAbility(
	// 	   FGameplayAbilitySpec(GameplayAbilityClass_Move, 1, INDEX_NONE, this));
	
	
	// give attack ability
	GameplayAbilitySpecHandle_Attack = AbilitySystemComponent->GiveAbility(
		   FGameplayAbilitySpec(GameplayAbilityClass_Attack, 1, INDEX_NONE, this));

	// calculate stats
	if (IsValid(StatsDataAsset))
	{	
		StatsDataAsset->GetStats(Level, UnitStatsSnapshot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Stats Data Asset Assigned To Unit: %s"), *this->GetName());
	}

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

int32 AGridUnit::GetAvailableMovement() const
{
	return UnitStatsSnapshot.Movement;
}
//
// bool AGridUnit::ActivateMovement_Implementation(FVector Location, const TMulticastDelegate<void(UGameplayAbility*)>::FDelegate& Callback)
// {
// 	if (!AbilitySystemComponent) { return false; }
//
// 	FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GameplayAbilitySpecHandle_Move);
// 	if (GameplayAbilitySpec->Ability == nullptr) { return false; }
//
// 	UE_LOG(LogTemp, Warning, TEXT("GridProxy Location: %s"), *Location.ToString());
// 	// set location as parameter on ability???
// 	MoveAbilityLocation = Location;
// 	UE_LOG(LogTemp, Warning, TEXT("GridProxy Location: %s"), *MoveAbilityLocation.ToString());	
// 		
// 	// bind to end callback
// 	// GameplayAbilitySpec->Ability->OnGameplayAbilityEnded.Clear();
// 	// TMulticastDelegate<void(UGameplayAbility*)>::FDelegate InDelegate;
// 	GameplayAbilitySpec->Ability->OnGameplayAbilityEnded.Add(Callback);
// 	// GameplayAbilitySpec->Ability->OnGameplayAbilityEnded.AddLambda([&Callback, GameplayAbilitySpec](UGameplayAbility* Ability)
// 	// {
// 	// 	if (Callback.IsBound())
// 	// 	{
// 	// 		Callback.ExecuteIfBound();
// 	// 	}
// 	// });
//
// 	// activate ability
// 	return AbilitySystemComponent->TryActivateAbility(GameplayAbilitySpecHandle_Move);	
// }

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
	if (!AbilitySystemComponent) { return false; }
	
	FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(GameplayAbilitySpecHandle_Attack);
	if (GameplayAbilitySpec->Ability == nullptr) { return false; }

	// set location as parameter on ability???
	MoveAbilityLocation = Location;
	
	// activate ability
	return AbilitySystemComponent->TryActivateAbility(GameplayAbilitySpecHandle_Attack);
}

FName AGridUnit::GetFaction() const
{
	// TODO: for now...
	const FString F = "Faction_" + FString::FromInt(Faction);
	return FName(F);
}

TArray<UWeapon*> AGridUnit::GetEquippedWeapons() const
{
	return EquippedWeapons;
}

TSet<int32> AGridUnit::GetWeaponRanges() const
{
	TSet<int32> OutValues;
	for (const auto Weapon : EquippedWeapons)
	{
		OutValues.Add(Weapon->GetRange());
	}

	return OutValues;
}

FUnitStatsSnapshot AGridUnit::GetSnapshot() const
{
	return UnitStatsSnapshot;
}

void AGridUnit::UpdateStats(const FUnitStatsSnapshot& StatAdjustments)
{
	// TODO: this is so sketch
	UnitStatsSnapshot += StatAdjustments;
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

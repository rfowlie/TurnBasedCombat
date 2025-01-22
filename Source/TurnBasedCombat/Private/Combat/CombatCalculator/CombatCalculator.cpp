// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/CombatCalculator.h"

#include "Combat/CombatCalculator/WeaponSolver.h"
#include "Grid/Manager/GridManager.h"
#include "Grid/Tile/GridTile.h"
#include "Grid/Unit/GridUnit.h"


void UCombatCalculator::SetEnabled() const
{
	if (OnEnabled.IsBound()) { OnEnabled.Broadcast(); }
}

void UCombatCalculator::SetDisabled() const
{
	if (OnDisabled.IsBound()) { OnDisabled.Broadcast(); }
}

void UCombatCalculator::SetInstigator(AGridUnit* Unit)
{
	if (IsValid(Unit))
	{
		InstigatorUnit = Unit;
		CalculateCombatSnapshot(InstigatorUnit, InstigatorTile, InstigatorSnapshot);
		if (OnInstigatorChanged.IsBound()) { OnInstigatorChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetTarget(AGridUnit* Unit)
{
	if (IsValid(Unit))
	{
		TargetUnit = Unit;
		CalculateCombatSnapshot(TargetUnit, TargetTile, TargetSnapshot);
		if (OnTargetChanged.IsBound()) { OnTargetChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetInstigatorTile(AGridTile* Tile)
{
	if (IsValid(Tile))
	{
		InstigatorTile = Tile;
		CalculateCombatSnapshot(InstigatorUnit, InstigatorTile, InstigatorSnapshot);
		if (OnInstigatorTileChanged.IsBound()) { OnInstigatorTileChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetTargetTile(AGridTile* Tile)
{
	if (IsValid(Tile) && TargetTile != Tile)
	{
		TargetTile = Tile;
		CalculateCombatSnapshot(TargetUnit, TargetTile, TargetSnapshot);
		if (OnTargetTileChanged.IsBound()) { OnTargetTileChanged.Broadcast(); }
	}
}

void UCombatCalculator::CalculateCombatOrder()
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }
	CombatOrder.Empty();

	// TODO: account for skills that alter the order of combat
	CombatOrder.Enqueue(InstigatorUnit);
	CombatOrder.Enqueue(TargetUnit);
	if (4 < InstigatorSnapshot.AttackSpeed - TargetSnapshot.AttackSpeed)
	{
		CombatOrder.Enqueue(InstigatorUnit);
	}
	else if (4 < TargetSnapshot.AttackSpeed - InstigatorSnapshot.AttackSpeed)
	{
		CombatOrder.Enqueue(TargetUnit);
	}
}

void UCombatCalculator::InitiateCombat()
{
	if (bCombatLock) { return; }
	bCombatLock = true;
	
	if (OnCombatBegin.IsBound()) { OnCombatBegin.Broadcast(); }
	
	// // subscribe to end event and notify grid unit to move
	// InstigatorUnit->OnEventAttackEnd.AddUniqueDynamic(this, &ThisClass::PostEvent_Attack);
	// InstigatorUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddLambda([this](const FAbilityEndedData& Data)
	// {
	// 	
	// });
	//
	// // TODO: use a gameplay event instead
	// // Instigator->GridUnit->AttackEvent(Location->GridTile->GetPlacementLocation(), Target->GridUnit);
	//
	// // TODO: try and activate through gameplay event instead of this current way...
	// // create gameplay event information
	// // UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator->GridUnit);
	// // FGameplayAbilitySpecHandle AbilityHandle;
	// // FGameplayAbilityActorInfo ActorInfo = *AbilitySystemComponent->AbilityActorInfo.Get();
	// FGameplayEventData EventData;
	// EventData.Instigator = Instigator->GridUnit;
	// EventData.Target = Target->GridUnit;
	// UGridEventPayload_Move* GridEventPayload = UGridEventPayload_Move::CreatePayload(Location->GridTile->GetPlacementLocation());
	// EventData.OptionalObject = GridEventPayload;
	//
	// // send gameplay event
	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	// 	Instigator->GridUnit, TAG_Event_Grid_Attack, EventData);
	// // AbilitySystemComponent->TriggerAbilityFromGameplayEvent(
	// // 	AbilityHandle,
	// // 	&ActorInfo,
	// // 	TAG_Event_Grid_Attack,
	// // 	&EventData, 
	// // 	*AbilitySystemComponent);
}

void UCombatCalculator::NextAttacker()
{
	if (!CombatOrder.IsEmpty())
	{
		AGridUnit* Unit;
		CombatOrder.Dequeue(Unit);
		Unit->GetAbilitySystemComponent()->OnAbilityEnded.AddLambda([this](const FAbilityEndedData& Data)
		{
			NextAttacker();
		});
	}
	else
	{
		if (OnCombatEnd.IsBound()) { OnCombatEnd.Broadcast(); }
	}
}

int32 UCombatCalculator::CalculateAttackSpeed(AGridUnit* Unit) const
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	FWeaponTraits WeaponTraits;
	GetWeapon(WeaponTraits, Unit->GetEquippedWeapon());
	return Speed - FMath::Max(0, WeaponTraits.Weight - Speed);
}

int32 UCombatCalculator::CalculateHitRate(AGridUnit* Unit) const
{
	FWeaponTraits WeaponTraits;
	GetWeapon(WeaponTraits, Unit->GetEquippedWeapon());
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill*2 + Luck + WeaponTraits.HitRate;
}

int32 UCombatCalculator::CalculateAvoid(AGridUnit* Unit, AGridTile* Tile) const
{
	return Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute()) +
		Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute()) / 2 +
		Tile->TerrainDataAsset->TerrainStats.Avoid;
}

int32 UCombatCalculator::CalculateCriticalRate(AGridUnit* Unit) const
{
	FWeaponTraits WeaponTraits;
	GetWeapon(WeaponTraits, Unit->GetEquippedWeapon());
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill/2 + Luck/4 + WeaponTraits.CriticalBonus;
}

int32 UCombatCalculator::CalculateCriticalAvoid(AGridUnit* Unit, AGridTile* Tile) const
{
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	const int32 TerrainAvoid = Tile->TerrainDataAsset->TerrainStats.Avoid;
	return Luck + TerrainAvoid/4;
}

void UCombatCalculator::CalculateCombatSnapshot(AGridUnit* Unit, AGridTile* Tile, FUnitCombatSnapshot& Snapshot) const
{
	Snapshot.Strength = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	Snapshot.Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	Snapshot.Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	Snapshot.Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	Snapshot.Defence = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	Snapshot.Resistance = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetResistanceAttribute());
	Snapshot.Movement = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetMovementAttribute());
	Snapshot.Constitution = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetConstitutionAttribute());
	Snapshot.AttackSpeed = CalculateAttackSpeed(Unit);
	Snapshot.HitRate = CalculateAttackSpeed(Unit);
	Snapshot.Avoid = CalculateAvoid(Unit, Tile);
	Snapshot.CriticalRate = CalculateCriticalRate(Unit);
	Snapshot.CriticalAvoid = CalculateCriticalAvoid(Unit, Tile);
}

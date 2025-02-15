// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/CombatCalculator.h"
#include "Combat/CombatData.h"
#include "Combat/Weapon/WeaponData.h"
#include "Grid/Manager/GridManager.h"
#include "Tile/GridTile.h"
#include "Unit/GridUnit.h"


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
		CalculateCombatSnapshot_Internal(InstigatorSnapshot, InstigatorUnit);
		CalculateCombatOrder();
		if (OnInstigatorChanged.IsBound()) { OnInstigatorChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetTarget(AGridUnit* Unit)
{
	if (IsValid(Unit))
	{
		TargetUnit = Unit;
		CalculateCombatSnapshot_Internal(TargetSnapshot, TargetUnit);
		CalculateCombatOrder();
		if (OnTargetChanged.IsBound()) { OnTargetChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetInstigatorTile(AGridTile* Tile)
{
	if (IsValid(Tile))
	{
		InstigatorTile = Tile;
		CalculateCombatSnapshot_Internal(InstigatorSnapshot, InstigatorUnit);
		CalculateCombatOrder();
		if (OnInstigatorTileChanged.IsBound()) { OnInstigatorTileChanged.Broadcast(); }
	}
}

void UCombatCalculator::SetTargetTile(AGridTile* Tile)
{
	if (IsValid(Tile) && TargetTile != Tile)
	{
		TargetTile = Tile;
		CalculateCombatSnapshot_Internal(TargetSnapshot, TargetUnit);
		CalculateCombatOrder();
		if (OnTargetTileChanged.IsBound()) { OnTargetTileChanged.Broadcast(); }
	}
}

void UCombatCalculator::CalculateCombatOrder()
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }
	CombatOrder.Empty();
	CombatOrderHelper.Empty();

	// TODO: account for skills that alter the order of combat
	CombatOrder.Enqueue(InstigatorUnit);
	CombatOrderHelper.Add(InstigatorUnit);
	CombatOrder.Enqueue(TargetUnit);
	CombatOrderHelper.Add(TargetUnit);
	if (4 < InstigatorSnapshot.AttackSpeed - TargetSnapshot.AttackSpeed)
	{
		CombatOrder.Enqueue(InstigatorUnit);
		CombatOrder.Enqueue(InstigatorUnit);
	}
	else if (4 < TargetSnapshot.AttackSpeed - InstigatorSnapshot.AttackSpeed)
	{
		CombatOrder.Enqueue(TargetUnit);
		CombatOrderHelper.Add(TargetUnit);
	}
}

const AGridUnit* UCombatCalculator::GetNextCombatant()
{
	AGridUnit* OutUnit;
	CombatOrder.Dequeue(OutUnit);
	return OutUnit;
}

void UCombatCalculator::InitiateCombat()
{
	if (bCombatLock) { return; }
	bCombatLock = true;
	
	if (OnCombatBegin.IsBound()) { OnCombatBegin.Broadcast(); }
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

int32 UCombatCalculator::CalculateAttackSpeed(AGridUnit* Unit, FWeaponTraits& WeaponTraits) const
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	return Speed - FMath::Max(0, WeaponTraits.Weight - Speed);
}

int32 UCombatCalculator::CalculateHitRate(AGridUnit* Unit, FWeaponTraits& WeaponTraits) const
{
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

int32 UCombatCalculator::CalculateCriticalRate(AGridUnit* Unit, FWeaponTraits& WeaponTraits) const
{
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

void UCombatCalculator::CalculateCombatSnapshot_Internal(
	FCombatSnapshot_Basic& OutSnapshot, AGridUnit* Unit)
{
	// should always reference the tile of the unit through grid manager?
	AGridTile* Tile = GridManager->GetGridTileOfUnit(Unit);
	
	// GetWeapon(OutSnapshot.WeaponTraits, Unit->GetEquippedWeapon());
	
	OutSnapshot.Strength = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	OutSnapshot.Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	OutSnapshot.Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	OutSnapshot.Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	OutSnapshot.Defence = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	OutSnapshot.Resistance = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetResistanceAttribute());
	OutSnapshot.Movement = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetMovementAttribute());
	OutSnapshot.Constitution = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetConstitutionAttribute());
	OutSnapshot.AttackSpeed = CalculateAttackSpeed(Unit, OutSnapshot.WeaponTraits);
	OutSnapshot.HitRate = CalculateHitRate(Unit, OutSnapshot.WeaponTraits);
	OutSnapshot.Avoid = CalculateAvoid(Unit, Tile);
	OutSnapshot.CriticalRate = CalculateCriticalRate(Unit, OutSnapshot.WeaponTraits);
	OutSnapshot.CriticalAvoid = CalculateCriticalAvoid(Unit, Tile);
}

bool UCombatCalculator::PopCombatOutcome(FCombatSnapshot_Advanced& CombatOutcome)
{
	AGridUnit* NextCombatant;
	CombatOrder.Dequeue(NextCombatant);
	if (!NextCombatant) { return false; }

	FCombatSnapshot_Basic AttackerSnapshot;
	CalculateCombatSnapshot_Internal(AttackerSnapshot, InstigatorUnit);
	FCombatSnapshot_Basic DefenderSnapshot;
	CalculateCombatSnapshot_Internal(DefenderSnapshot, TargetUnit);
	
	// if (NextCombatant == InstigatorUnit)
	// {
	// 	CombatOutcome.Instigator = InstigatorUnit;
	// 	CombatOutcome.Target = TargetUnit;
	// 	CalculateCombatOutcome(CombatOutcome, AttackerSnapshot, DefenderSnapshot);
	// }
	// else
	// {
	// 	CombatOutcome.Instigator = TargetUnit;
	// 	CombatOutcome.Target = InstigatorUnit;
	// 	CalculateCombatOutcome(CombatOutcome, DefenderSnapshot, AttackerSnapshot);
	// }

	return true;
}

void UCombatCalculator::GetCombatOutcomes(TArray<FCombatSnapshot_Advanced>& CombatOutcomes)
{
	for (const auto Combatant : CombatOrderHelper)
	{
		FCombatSnapshot_Basic AttackerSnapshot;
		CalculateCombatSnapshot_Internal(AttackerSnapshot, InstigatorUnit);
		FCombatSnapshot_Basic DefenderSnapshot;
		CalculateCombatSnapshot_Internal(DefenderSnapshot, TargetUnit);

		// FCombatSnapshot_Advanced TempOutcome;
		// if (Combatant == InstigatorUnit)
		// {
		// 	TempOutcome.Instigator = InstigatorUnit;
		// 	TempOutcome.Target = TargetUnit;
		// 	CalculateCombatOutcome(TempOutcome, AttackerSnapshot, DefenderSnapshot);
		// }
		// else
		// {
		// 	TempOutcome.Instigator = TargetUnit;
		// 	TempOutcome.Target = InstigatorUnit;
		// 	CalculateCombatOutcome(TempOutcome, DefenderSnapshot, AttackerSnapshot);
		// }

		// CombatOutcomes.Add(TempOutcome);
	}
	
	// TArray<AGridUnit*> BackInQueue;
	// while(!CombatOrder.IsEmpty())
	// {
	// 	AGridUnit* NextCombatant;
	// 	CombatOrder.Dequeue(NextCombatant);
	// 	if (!NextCombatant) { continue; }
	// 	
	// 	BackInQueue.Add(NextCombatant);
	//
	// 	FUnitCombatSnapshot AttackerSnapshot;
	// 	CalculateCombatSnapshot_Internal(AttackerSnapshot, InstigatorUnit);
	// 	FUnitCombatSnapshot DefenderSnapshot;
	// 	CalculateCombatSnapshot_Internal(DefenderSnapshot, TargetUnit);
	//
	// 	FCombatOutcome TempOutcome;
	// 	if (NextCombatant == InstigatorUnit)
	// 	{
	// 		TempOutcome.Instigator = InstigatorUnit;
	// 		TempOutcome.Target = TargetUnit;
	// 		CalculateCombatOutcome(TempOutcome, AttackerSnapshot, DefenderSnapshot);
	// 	}
	// 	else
	// 	{
	// 		TempOutcome.Instigator = TargetUnit;
	// 		TempOutcome.Target = InstigatorUnit;
	// 		CalculateCombatOutcome(TempOutcome, DefenderSnapshot, AttackerSnapshot);
	// 	}
	//
	// 	CombatOutcomes.Add(TempOutcome);
	// }
	//
	// // replace the units removed from queue...
	// for (auto Unit : BackInQueue)
	// {
	// 	CombatOrder.Enqueue(Unit);
	// }
}

void UCombatCalculator::CalculateCombatOutcome(
	FCombatSnapshot_Advanced& CombatOutcome, const FCombatSnapshot_Basic& Instigator, const FCombatSnapshot_Basic& Target) const
{
	// check if even hits, could also do partial hits...	
	CombatOutcome.HitChance = FMath::Clamp(Instigator.HitRate - Target.Avoid, 0, 100);
	UE_LOG(LogTemp, Log, TEXT("COMBAT CALCULATOR - Accuracy: %d"), CombatOutcome.HitChance);
	CombatOutcome.bHit = FMath::RandRange(0, 100) < CombatOutcome.HitChance;
	UE_LOG(LogTemp, Log, TEXT("COMBAT CALCULATOR - Hit: %d"), CombatOutcome.bHit);
	
	int32 AttackPower = Instigator.Strength + Instigator.WeaponTraits.Might;
	int32 DefensePower = Target.Defence;
	
	CombatOutcome.DamageDealt = FMath::Clamp(AttackPower - DefensePower, 0, GetMaxDamage());
	UE_LOG(LogTemp, Log, TEXT("COMBAT CALCULATOR - HealthChange: %d"), CombatOutcome.DamageDealt);
	
	CombatOutcome.CriticalChance = 0;
}

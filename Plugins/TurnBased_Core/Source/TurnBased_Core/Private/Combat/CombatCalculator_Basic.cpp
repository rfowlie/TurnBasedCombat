// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator_Basic.h"
#include "Combat/CombatData.h"
#include "GridSystemsUtility.h"
#include "Grid/GridWorldSubsystem.h"
#include "Tile/GridTile.h"
#include "Unit/GridUnit.h"


void UCombatCalculator_Basic::GetCombatPrediction(FCombatPrediction& OutCombatPrediction,
	const FCombatInformation& CombatInformation) const
{
	OutCombatPrediction.CombatInformation = CombatInformation;

	// always calculate basic information
	GetUnitSnapshotBasic(OutCombatPrediction.InstigatorSnapshotBasic, CombatInformation.InstigatorUnit, CombatInformation.InstigatorWeapon);
	GetUnitSnapshotBasic(OutCombatPrediction.TargetSnapshotBasic, CombatInformation.TargetUnit, CombatInformation.TargetWeapon);
	
	// const int32 CombatRange = UGridSystemsUtility::CalculateGridPosition(CombatInformation.InstigatorTile).GetDistance(UGridSystemsUtility::CalculateGridPosition(CombatInformation.TargetTile));
	FWeaponContainer WeaponContainer;
	GetWeaponByName(WeaponContainer, CombatInformation.InstigatorWeapon);
	const int32 CombatRange = WeaponContainer.WeaponTraits.Range;	

	OutCombatPrediction.CombatOrder.Empty();

	// only calculate advanced information if unit has correct weapon range
	if (OutCombatPrediction.InstigatorSnapshotBasic.WeaponTraits.Range == CombatRange)
	{
		GetUnitSnapshotAdvanced(OutCombatPrediction.InstigatorSnapShotAdvanced, OutCombatPrediction.InstigatorSnapshotBasic, OutCombatPrediction.TargetSnapshotBasic);
		OutCombatPrediction.CombatOrder.Add(CombatInformation.InstigatorUnit);
	}
	if (OutCombatPrediction.TargetSnapshotBasic.WeaponTraits.Range == CombatRange)
	{
		GetUnitSnapshotAdvanced(OutCombatPrediction.TargetSnapshotAdvanced, OutCombatPrediction.TargetSnapshotBasic, OutCombatPrediction.InstigatorSnapshotBasic);
		OutCombatPrediction.CombatOrder.Add(CombatInformation.TargetUnit);
	}

	// doubling
	if (OutCombatPrediction.InstigatorSnapshotBasic.WeaponTraits.Range == CombatRange && OutCombatPrediction.InstigatorSnapshotBasic.AttackSpeed - 4 > OutCombatPrediction.TargetSnapshotBasic.AttackSpeed)
	{
		OutCombatPrediction.CombatOrder.Add(CombatInformation.InstigatorUnit);
	}
	if (OutCombatPrediction.TargetSnapshotBasic.WeaponTraits.Range == CombatRange && OutCombatPrediction.TargetSnapshotBasic.AttackSpeed - 4 > OutCombatPrediction.InstigatorSnapshotBasic.AttackSpeed)
	{
		OutCombatPrediction.CombatOrder.Add(CombatInformation.TargetUnit);
	}
}

void UCombatCalculator_Basic::GetCombatOutcome(
	FCombatSnapshot_Outcome& Outcome, AGridUnit* InstigatorUnit, const FName WeaponName, AGridUnit* TargetUnit) const
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }

	Outcome.Instigator = InstigatorUnit;
	Outcome.Target = TargetUnit;
	
	FCombatSnapshot_Basic InstigatorSnapshot;
	GetUnitSnapshotBasic(InstigatorSnapshot, InstigatorUnit, WeaponName);
	FCombatSnapshot_Basic TargetSnapshot;
	GetUnitSnapshotBasic(TargetSnapshot, TargetUnit, TargetUnit->GetEquippedWeaponName());

	// THE HARD WAY
	// check if target can attack first (special attributes or skills, etc.)
	// do instigator attack
	// check if target still alive (sub damage from health)
	// check if target can attack back (weapon range correct, etc.)
	// do target attack
	// check if even hits, could also do partial hits...

	// THE EASY WAY
	// get rough outcomes for instigator->target & target->instigator
	FCombatSnapshot_Advanced InstigatorOutcome;
	GetUnitSnapshotAdvanced(InstigatorOutcome, InstigatorSnapshot, TargetSnapshot);
	FCombatSnapshot_Advanced TargetOutcome;
	GetUnitSnapshotAdvanced(TargetOutcome, TargetSnapshot, InstigatorSnapshot);
	
	Outcome.InstigatorSnapshot = InstigatorOutcome;
	Outcome.TargetSnapshot = TargetOutcome;

	// FOR NOW
	// check how many attacks each unit can potentially get
	// const int32 CombatRange = UGridHelper::CalculateGridPosition(InstigatorUnit).GetDistance(UGridHelper::CalculateGridPosition(TargetUnit));
	// Outcome.InstigatorAttacks = InstigatorSnapshot.WeaponTraits.Range == CombatRange ? 1 : 0;
	// Outcome.TargetAttacks = TargetSnapshot.WeaponTraits.Range == CombatRange ? 1 : 0;
	// // DO FE IF ATTACK SPEED GREATER BY 4+
	// Outcome.InstigatorAttacks *= InstigatorSnapshot.AttackSpeed - 4 > TargetSnapshot.Speed ? 2 : 1;
	// Outcome.TargetAttacks *= TargetSnapshot.AttackSpeed - 4 > InstigatorSnapshot.Speed ? 2 : 1;

	// attack order
	const int32 CombatRange = UGridSystemsUtility::CalculateGridPosition(InstigatorUnit).GetDistance(UGridSystemsUtility::CalculateGridPosition(TargetUnit));
	Outcome.CombatOrder.Empty();
	if (InstigatorSnapshot.WeaponTraits.Range == CombatRange)
	{
		Outcome.CombatOrder.Add(InstigatorUnit);
	}
	if (TargetSnapshot.WeaponTraits.Range == CombatRange)
	{
		Outcome.CombatOrder.Add(TargetUnit);
	}

	// extra speed attack
	if (InstigatorSnapshot.WeaponTraits.Range == CombatRange && InstigatorSnapshot.AttackSpeed - 4 > TargetSnapshot.AttackSpeed)
	{
		Outcome.CombatOrder.Add(InstigatorUnit);
	}
	else if (TargetSnapshot.WeaponTraits.Range == CombatRange && TargetSnapshot.AttackSpeed - 4 > InstigatorSnapshot.AttackSpeed)
	{
		Outcome.CombatOrder.Add(TargetUnit);
	}
}

TArray<AGridUnit*> UCombatCalculator_Basic::GetCombatOrder(FCombatSnapshot_Outcome& Outcome)
{
	// // FOR NOW
	// // check how many attacks each unit can potentially get
	// const int32 CombatRange = UGridHelper::CalculateGridPosition(InstigatorUnit).GetDistance(UGridHelper::CalculateGridPosition(TargetUnit));
	// Outcome.InstigatorAttacks = Outcome.InstigatorSnapshot.WeaponTraits.Range == CombatRange ? 1 : 0;
	// Outcome.TargetAttacks = Outcome.TargetSnapshot.WeaponTraits.Range == CombatRange ? 1 : 0;
	// // DO FE IF ATTACK SPEED GREATER BY 4+
	// Outcome.InstigatorAttacks *= Outcome.InstigatorSnapshot.AttackSpeed - 4 > Outcome.TargetSnapshot.Speed ? 2 : 1;
	// Outcome.TargetAttacks *= Outcome.TargetSnapshot.AttackSpeed - 4 > Outcome.InstigatorSnapshot.Speed ? 2 : 1;
	//
	// // attack order
	// if (Outcome.InstigatorSnapshot.WeaponTraits.Range == CombatRange) { Outcome.CombatOrder.Add(InstigatorUnit); }
	// if (Outcome.TargetSnapshot.WeaponTraits.Range == CombatRange) { Outcome.CombatOrder.Add(TargetUnit); }
	//
	// // extra speed attack
	// if (Outcome.InstigatorSnapshot.WeaponTraits.Range == CombatRange && Outcome.InstigatorSnapshot.AttackSpeed - 4 > Outcome.TargetSnapshot.Speed)
	// {
	// 	Outcome.CombatOrder.Add(InstigatorUnit);
	// }
	// else if (Outcome.TargetSnapshot.WeaponTraits.Range == CombatRange && Outcome.TargetSnapshot.AttackSpeed - 4 > Outcome.InstigatorSnapshot.Speed)
	// {
	// 	Outcome.CombatOrder.Add(TargetUnit);
	// }

	return TArray<AGridUnit*>();
}

void UCombatCalculator_Basic::GetUnitSnapshotBasic(
	FCombatSnapshot_Basic& OutSnapshot, AGridUnit* InGridUnit, const FName WeaponName) const
{
	if (!IsValid(InGridUnit)) { return; }

	AGridTileBase* Tile = nullptr;
	if (UGridWorldSubsystem* GridSubsystem = InGridUnit->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		Tile = GridSubsystem->GetGridTileOfUnit(InGridUnit);
	}

	if (!IsValid(Tile)) { return; }

	FWeaponContainer WeaponContainer;
	// GetWeaponByName(WeaponContainer, InGridUnit->GetEquippedWeaponName());
	GetWeaponByName(WeaponContainer, WeaponName);
	OutSnapshot.WeaponTraits = WeaponContainer.WeaponTraits;

	OutSnapshot.Health =  InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetHealthAttribute());
	OutSnapshot.Strength = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetStrengthAttribute());
	OutSnapshot.Skill = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	OutSnapshot.Speed = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	OutSnapshot.Luck = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	OutSnapshot.Defence = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetDefenceAttribute());
	OutSnapshot.Resistance = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetResistanceAttribute());
	OutSnapshot.Movement = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetMovementAttribute());
	OutSnapshot.Constitution = InGridUnit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetConstitutionAttribute());
	OutSnapshot.AttackSpeed = CalculateAttackSpeed(InGridUnit, OutSnapshot.WeaponTraits);
	OutSnapshot.HitRate = CalculateHitRate(InGridUnit, OutSnapshot.WeaponTraits);
	OutSnapshot.Avoid = CalculateAvoid(OutSnapshot, InGridUnit, Tile);
	OutSnapshot.CriticalRate = CalculateCriticalRate(InGridUnit, OutSnapshot.WeaponTraits);
	OutSnapshot.CriticalAvoid = CalculateCriticalAvoid(InGridUnit, Tile);
}

void UCombatCalculator_Basic::GetUnitSnapshotAdvanced(
	FCombatSnapshot_Advanced& OutSnapshot,
	const FCombatSnapshot_Basic& InstigatorSnapshot,
	const FCombatSnapshot_Basic& TargetSnapshot) const
{
	OutSnapshot.HitChance = FMath::Clamp(InstigatorSnapshot.HitRate - TargetSnapshot.Avoid, 0, 100);
	OutSnapshot.bHit = FMath::RandRange(0, 100) < OutSnapshot.HitChance;
	OutSnapshot.DamageDealt = FMath::Clamp(InstigatorSnapshot.Strength + InstigatorSnapshot.WeaponTraits.Might - TargetSnapshot.Defence, 0, GetMaxDamage());
	OutSnapshot.CriticalChance = FMath::Clamp(0, 100, InstigatorSnapshot.CriticalRate - TargetSnapshot.CriticalAvoid);

	// are we using this??? 
	OutSnapshot.AttackPower = OutSnapshot.DamageDealt;
}

TArray<int32> UCombatCalculator_Basic::GetWeaponRangesByName(const TArray<FName>& WeaponNames) const
{
	TArray<int32> Ranges;
	for (FName WeaponName : WeaponNames)
	{
		FWeaponContainer WeaponContainer;
		GetWeaponByName(WeaponContainer, WeaponName);
		Ranges.Add(WeaponContainer.WeaponTraits.Range);
	}

	return Ranges;
}

int32 UCombatCalculator_Basic::CalculateAttackSpeed(const AGridUnit* Unit, FWeaponTraits& WeaponTraits)
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	return Speed - FMath::Max(0,Speed - WeaponTraits.Weight);
}

int32 UCombatCalculator_Basic::CalculateHitRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits)
{
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill*2 + Luck + WeaponTraits.HitRate;
}

int32 UCombatCalculator_Basic::CalculateAvoid(
	const FCombatSnapshot_Basic& Snapshot, const AGridUnit* Unit, const AGridTile* Tile)
{
	return Snapshot.AttackSpeed * 2 + Snapshot.Luck / 2 + Tile->TerrainDataAsset->TerrainStats.Avoid;
}

int32 UCombatCalculator_Basic::CalculateCriticalRate(const AGridUnit* Unit, FWeaponTraits& WeaponTraits)
{
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill/2 + Luck/4 + WeaponTraits.CriticalBonus;
}

int32 UCombatCalculator_Basic::CalculateCriticalAvoid(const AGridUnit* Unit, const AGridTile* Tile)
{
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	const int32 TerrainAvoid = Tile->TerrainDataAsset->TerrainStats.Avoid;
	return Luck + TerrainAvoid/4;
}

void UCombatCalculator_Basic::CalculateCombatScore(FCombatScore& CombatScore) const
{
	FCombatSnapshot_Basic InstigatorSnapshotBasic;
	GetUnitSnapshotBasic(InstigatorSnapshotBasic, CombatScore.InstigatorUnit, CombatScore.InstigatorWeapon);
	FCombatSnapshot_Basic TargetSnapshotBasic;
	GetUnitSnapshotBasic(TargetSnapshotBasic, CombatScore.TargetUnit, CombatScore.TargetWeapon);
	FCombatSnapshot_Advanced InstigatorSnapShotAdvanced;
	GetUnitSnapshotAdvanced(InstigatorSnapShotAdvanced, InstigatorSnapshotBasic, TargetSnapshotBasic);
	FCombatSnapshot_Advanced TargetSnapShotAdvanced;
	GetUnitSnapshotAdvanced(TargetSnapShotAdvanced, TargetSnapshotBasic, InstigatorSnapshotBasic);

	// FOR NOW... do not worry about speed, just give a score to the outcome
	float Score = 0;
	Score += InstigatorSnapShotAdvanced.HitChance / 100.f;
	Score += InstigatorSnapShotAdvanced.CriticalChance / 100.f * 2.f;
	Score += InstigatorSnapShotAdvanced.DamageDealt / TargetSnapshotBasic.Health;
	Score += TargetSnapShotAdvanced.DamageDealt / InstigatorSnapshotBasic.Health;
	CombatScore.Score = Score;
}

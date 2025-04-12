// Fill out your copyright notice in the Description page of Project Settings.


#include "_FireEmblemLike/Combat/FireEmblemCombatCalculator.h"
#include "Grid/GridWorldSubsystem.h"
#include "Grid/GridHelper.h"
#include "_FireEmblemLike/Combat/FireEmblemCombatStructs.h"
#include "_FireEmblemLike/Unit/FireEmblemUnit.h"
#include "_FireEmblemLike/Unit/FireEmblemUnitInterface.h"


void UFireEmblemCombatCalculator::CalculateAllCombatPredictions(
	FFireEmblemCombatPrediction& OutCombatPrediction, AFireEmblemUnit* ActiveUnit)
{	
	UGridWorldSubsystem* GridWorldSubsystem = GetWorld()->GetSubsystem<UGridWorldSubsystem>();
	
	TArray<FFireEmblemCombatPrediction> CombatPredictions;
	TMap<AFireEmblemUnit*, FGridTileArray> PossibleAttacksMap = GridWorldSubsystem->GetEnemiesInRangeWithAttackTiles(ActiveUnit);

	// get all possible combats (predictions)
	for (auto GridUnitTilePair : PossibleAttacksMap)
	{
		AFireEmblemUnit* FireEmblemUnit = Cast<AFireEmblemUnit>(GridUnitTilePair.Key);
		if (!FireEmblemUnit) { continue; }
		
		AFireEmblemUnit* TargetsGridTile = GridWorldSubsystem->GetGridTileOfUnit(GridUnitTilePair.Key);
		const FName TargetWeapon = IFireEmblemUnitInterface::Execute_GetEquippedWeaponName(GridUnitTilePair.Key);
		for (const auto GridTile : GridUnitTilePair.Value.GridTiles)
		{
			for (const FName Weapon : ActiveUnit->GetWeaponsInMap())
			{
				FFireEmblemCombatPrediction TempCombatPrediction;
				FFireEmblemCombatInformation TempCombatInformation;
				TempCombatInformation.InstigatorUnit = ActiveUnit;
				TempCombatInformation.InstigatorTile = GridTile;
				TempCombatInformation.InstigatorWeapon = Weapon;
				TempCombatInformation.TargetUnit = FireEmblemUnit;
				TempCombatInformation.TargetTile = TargetsGridTile;
				TempCombatInformation.TargetWeapon = TargetWeapon;
				GetCombatPredictionFromInformation(TempCombatPrediction, TempCombatInformation);
				
				// don't allow it as an option if neither unit can attack or instigator cannot reach with weapon range
				if (!OutCombatPrediction.CombatOrder.IsEmpty() && OutCombatPrediction.CombatOrder.Contains(OutCombatPrediction.CombatInformation.InstigatorUnit))
				{
					CombatPredictions.Add(OutCombatPrediction);
				}				
			}
		}
	}

	if (!CombatPredictions.IsEmpty())
	{
		// determine best action to take
		for (FFireEmblemCombatPrediction& Prediction : CombatPredictions)
		{
			FFireEmblemCombatEvaluation ObjectiveCombatEvaluation;
			GetCombatEvaluation(ObjectiveCombatEvaluation, Prediction);
			FFireEmblemCombatEvaluation UnitCombatEvaluation;
			// Prediction.CombatInformation.InstigatorUnit->CombatBehaviourComponent->GetCombatEvaluation(UnitCombatEvaluation, Prediction);

			// assign evaluation to prediction
			Prediction.CombatEvaluation = ObjectiveCombatEvaluation * UnitCombatEvaluation;
		}

		// CombatPredictions.Sort();
		Algo::Sort(CombatPredictions);
		OutCombatPrediction = CombatPredictions[0];
	}
}

void UFireEmblemCombatCalculator::GetCombatPredictionFromInformation(FFireEmblemCombatPrediction& OutCombatPrediction,
	const FFireEmblemCombatInformation& CombatInformation) const
{
	OutCombatPrediction.CombatInformation = CombatInformation;

	// always calculate basic information
	GetUnitSnapshotBasic(OutCombatPrediction.InstigatorSnapshotBasic, CombatInformation.InstigatorUnit, CombatInformation.InstigatorWeapon);
	GetUnitSnapshotBasic(OutCombatPrediction.TargetSnapshotBasic, CombatInformation.TargetUnit, CombatInformation.TargetWeapon);
	
	const int32 CombatRange = UGridHelper::CalculateGridPosition(CombatInformation.InstigatorTile).GetDistance(UGridHelper::CalculateGridPosition(CombatInformation.TargetTile));
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

void UFireEmblemCombatCalculator::GetCombatOutcome(
	FFireEmblemCombatOutcome& Outcome, AFireEmblemUnit* InstigatorUnit, const FName WeaponName, AFireEmblemUnit* TargetUnit) const
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }

	Outcome.Instigator = InstigatorUnit;
	Outcome.Target = TargetUnit;
	
	FFireEmblemStatsBasic InstigatorSnapshot;
	GetUnitSnapshotBasic(InstigatorSnapshot, InstigatorUnit, WeaponName);
	FFireEmblemStatsBasic TargetSnapshot;
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
	FFireEmblemStatsAdvanced InstigatorOutcome;
	GetUnitSnapshotAdvanced(InstigatorOutcome, InstigatorSnapshot, TargetSnapshot);
	FFireEmblemStatsAdvanced TargetOutcome;
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
	const int32 CombatRange = UGridHelper::CalculateGridPosition(InstigatorUnit).GetDistance(UGridHelper::CalculateGridPosition(TargetUnit));
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

TArray<AFireEmblemUnit*> UFireEmblemCombatCalculator::GetCombatOrder(FFireEmblemCombatOutcome& Outcome)
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

	return TArray<AFireEmblemUnit*>();
}

void UFireEmblemCombatCalculator::GetUnitSnapshotBasic(
	FFireEmblemStatsBasic& OutSnapshot, AFireEmblemUnit* InGridUnit, const FName WeaponName) const
{
	if (!IsValid(InGridUnit)) { return; }

	AGridTile* Tile = nullptr;
	if (UGridWorldSubsystem* GridSubsystem = InGridUnit->GetWorld()->GetSubsystem<UGridWorldSubsystem>())
	{
		// Tile = GridSubsystem->GetGridTileOfUnit(InGridUnit);
	}

	if (!IsValid(Tile)) { return; }

	FFireEmblemWeaponContainer WeaponContainer;
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

void UFireEmblemCombatCalculator::GetUnitSnapshotAdvanced(
	FFireEmblemStatsAdvanced& OutSnapshot,
	const FFireEmblemStatsBasic& InstigatorSnapshot,
	const FFireEmblemStatsBasic& TargetSnapshot) const
{
	OutSnapshot.HitChance = FMath::Clamp(InstigatorSnapshot.HitRate - TargetSnapshot.Avoid, 0, 100);
	OutSnapshot.bHit = FMath::RandRange(0, 100) < OutSnapshot.HitChance;
	OutSnapshot.DamageDealt = FMath::Clamp(InstigatorSnapshot.Strength + InstigatorSnapshot.WeaponTraits.Might - TargetSnapshot.Defence, 0, GetMaxDamage());
	OutSnapshot.CriticalChance = FMath::Clamp(0, 100, InstigatorSnapshot.CriticalRate - TargetSnapshot.CriticalAvoid);

	// are we using this??? 
	OutSnapshot.AttackPower = OutSnapshot.DamageDealt;
}

TArray<int32> UFireEmblemCombatCalculator::GetWeaponRangesByName(const TArray<FName>& WeaponNames) const
{
	TArray<int32> Ranges;
	for (FName WeaponName : WeaponNames)
	{
		FFireEmblemWeaponContainer WeaponContainer;
		GetWeaponByName(WeaponContainer, WeaponName);
		Ranges.Add(WeaponContainer.WeaponTraits.Range);
	}

	return Ranges;
}

int32 UFireEmblemCombatCalculator::CalculateAttackSpeed(const AFireEmblemUnit* Unit, FFireEmblemWeaponTraits& WeaponTraits)
{
	int32 Speed = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSpeedAttribute());
	return Speed - FMath::Max(0,Speed - WeaponTraits.Weight);
}

int32 UFireEmblemCombatCalculator::CalculateHitRate(const AFireEmblemUnit* Unit, FFireEmblemWeaponTraits& WeaponTraits)
{
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill*2 + Luck + WeaponTraits.HitRate;
}

int32 UFireEmblemCombatCalculator::CalculateAvoid(
	const FFireEmblemStatsBasic& Snapshot, const AFireEmblemUnit* Unit, const AGridTile* Tile)
{
	return Snapshot.AttackSpeed * 2 + Snapshot.Luck / 2 + Tile->TerrainDataAsset->TerrainStats.Avoid;
}

int32 UFireEmblemCombatCalculator::CalculateCriticalRate(const AFireEmblemUnit* Unit, FFireEmblemWeaponTraits& WeaponTraits)
{
	const int32 Skill = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetSkillAttribute());
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	return Skill/2 + Luck/4 + WeaponTraits.CriticalBonus;
}

int32 UFireEmblemCombatCalculator::CalculateCriticalAvoid(const AFireEmblemUnit* Unit, const AGridTile* Tile)
{
	const int32 Luck = Unit->GetAbilitySystemComponent()->GetNumericAttribute(UGridUnitAttributeSet::GetLuckAttribute());
	const int32 TerrainAvoid = Tile->TerrainDataAsset->TerrainStats.Avoid;
	return Luck + TerrainAvoid/4;
}

void UFireEmblemCombatCalculator::CalculateCombatScore(FFireEmblemCombatScore& CombatScore) const
{
	FFireEmblemStatsBasic InstigatorSnapshotBasic;
	GetUnitSnapshotBasic(InstigatorSnapshotBasic, CombatScore.InstigatorUnit, CombatScore.InstigatorWeapon);
	FFireEmblemStatsBasic TargetSnapshotBasic;
	GetUnitSnapshotBasic(TargetSnapshotBasic, CombatScore.TargetUnit, CombatScore.TargetWeapon);
	FFireEmblemStatsAdvanced InstigatorSnapShotAdvanced;
	GetUnitSnapshotAdvanced(InstigatorSnapShotAdvanced, InstigatorSnapshotBasic, TargetSnapshotBasic);
	FFireEmblemStatsAdvanced TargetSnapShotAdvanced;
	GetUnitSnapshotAdvanced(TargetSnapShotAdvanced, TargetSnapshotBasic, InstigatorSnapshotBasic);

	// FOR NOW... do not worry about speed, just give a score to the outcome
	float Score = 0;
	Score += InstigatorSnapShotAdvanced.HitChance / 100.f;
	Score += InstigatorSnapShotAdvanced.CriticalChance / 100.f * 2.f;
	Score += InstigatorSnapShotAdvanced.DamageDealt / TargetSnapshotBasic.Health;
	Score += TargetSnapShotAdvanced.DamageDealt / InstigatorSnapshotBasic.Health;
	CombatScore.Score = Score;
}

void UFireEmblemCombatCalculator::GetCombatEvaluation(FFireEmblemCombatEvaluation& OutCombatEvaluation,
	const FFireEmblemCombatPrediction& InCombatPrediction)
{
	// CombatPrediction = InCombatPrediction;
	OutCombatEvaluation.HitChanceInstigator = HitChanceInstigator(InCombatPrediction);
	OutCombatEvaluation.HitChanceTarget = HitChanceTarget(InCombatPrediction);
	OutCombatEvaluation.CriticalChanceInstigator = CriticalChanceInstigator(InCombatPrediction);
	OutCombatEvaluation.CriticalChanceTarget = CriticalChanceTarget(InCombatPrediction);
	OutCombatEvaluation.HealthChangePercentageInstigator = HealthChangePercentageInstigator(InCombatPrediction);
	OutCombatEvaluation.HealthChangePercentageTarget = HealthChangePercentageTarget(InCombatPrediction);
	OutCombatEvaluation.DefeatedInstigator = DefeatedInstigator(InCombatPrediction);
	OutCombatEvaluation.DefeatedTarget = DefeatedTarget(InCombatPrediction);
}

float UFireEmblemCombatCalculator::HitChanceInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.InstigatorSnapShotAdvanced.HitChance / 100.f, 1.5f);
}

float UFireEmblemCombatCalculator::HitChanceTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.TargetSnapshotAdvanced.HitChance / 100.f, 1.5f);
}

float UFireEmblemCombatCalculator::CriticalChanceInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.InstigatorSnapShotAdvanced.CriticalChance / 100.f, 0.7);
}

float UFireEmblemCombatCalculator::CriticalChanceTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	return FMath::Pow(InCombatPrediction.TargetSnapshotAdvanced.CriticalChance / 100.f, 0.7);
}

float UFireEmblemCombatCalculator::HealthChangePercentageInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	if (InCombatPrediction.InstigatorSnapshotBasic.Health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UActionEvaluator_Combat::HealthChangePercentageInstigator - health zero"))
		return 0.f;
	}
	return InCombatPrediction.TargetSnapshotAdvanced.DamageDealt / InCombatPrediction.InstigatorSnapshotBasic.Health;
}

float UFireEmblemCombatCalculator::HealthChangePercentageTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	if (InCombatPrediction.TargetSnapshotBasic.Health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ActionEvaluator_Combat::HealthChangePercentageTarget - health zero"))
		return 0.f;
	}
	return InCombatPrediction.InstigatorSnapShotAdvanced.DamageDealt / InCombatPrediction.TargetSnapshotBasic.Health;
}

float UFireEmblemCombatCalculator::DefeatedInstigator(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	return InCombatPrediction.TargetSnapshotAdvanced.DamageDealt < InCombatPrediction.InstigatorSnapshotBasic.Health ?
		1.0f : InCombatPrediction.TargetSnapshotAdvanced.HitChance;
}

float UFireEmblemCombatCalculator::DefeatedTarget(const FFireEmblemCombatPrediction& InCombatPrediction) const
{
	return InCombatPrediction.InstigatorSnapShotAdvanced.DamageDealt < InCombatPrediction.TargetSnapshotBasic.Health ?
		1.0f : InCombatPrediction.InstigatorSnapShotAdvanced.HitChance;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridWorldSubsystem.h"

#include "Combat/CombatCalculator_Basic.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameMode/GameMode_TurnBased_Combat.h"
#include "Grid/GridHelper.h"
#include "Tile/GridTile.h"
#include "Turn/TurnWorldSubsystem.h"
#include "Unit/GridUnit.h"



void UGridWorldSubsystem::RegisterGridTile(AGridTile* GridTile)
{
	if (IsValid(GridTile) && !GridTilesAll.Contains(GridTile))
	{
		GridTilesAll.AddUnique(GridTile);
		
		// add to map
		const FGridPosition GridPosition = UGridHelper::CalculateGridPosition(GridTile);
		LocationGridTileMap.Add(GridPosition, GridTile);
		GridTileLocationMap.Add(GridTile, GridPosition);
		
		// bind to events
		GridTile->OnBeginCursorOver.AddDynamic(this, &ThisClass::OnBeginCursorOverGridTile);

		UE_LOG(LogTemp, Log, TEXT("GridWorldSubsystem - RegisterGridTile: %s"), *GridTile->GetName());
	}
}

void UGridWorldSubsystem::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (IsValid(GridUnit) && !GridUnitsAll.Contains(GridUnit))
	{
		GridUnitsAll.AddUnique(GridUnit);
		
		// add to map
		FGridPosition GridPosition = UGridHelper::CalculateGridPosition(GridUnit);
		
		// TODO: why the double map???
		LocationGridUnitMap.Add(GridPosition, GridUnit);
		GridUnitLocationMap.Add(GridUnit, GridPosition);

		// GridUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddLambda([this, GridUnit](const FAbilityEndedData& Data)
		// {
		// 	if (Data.AbilitySpecHandle == GridUnit->GameplayAbilitySpecHandle_Move)
		// 	{
		// 		PostEvent_Move(GridUnit);
		// 	}
		// });

		// whenever a unit does something, add it to the active units list
		// GridUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(
		// 	this, &ThisClass::OnGridUnitAbilityActivated, GridUnit);
		// GridUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(
		// 	this, &ThisClass::OnGridUnitAbilityEnded, GridUnit);
		GridUnit->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(
			this, &ThisClass::OnGridUnitAbilityActivated);
		GridUnit->GetAbilitySystemComponent()->AbilityEndedCallbacks.AddUObject(
			this, &ThisClass::OnGridUnitAbilityEnded);
		
		// GridUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddLambda([this, GridUnit](const FAbilityEndedData& Data)
		// {
		// 	if (Data.AbilitySpecHandle == GridUnit->GameplayAbilitySpecHandle_Move)
		// 	{
		// 		PostEvent_Move(GridUnit);
		// 	}
		// });
	}
}

void UGridWorldSubsystem::OnBeginCursorOverGridTile(AActor* Actor)
{
	if (AGridTile* GridTile = Cast<AGridTile>(Actor); IsValid(GridTile))
	{		
		// update tile
		if (IsValid(GridTileHovered))
		{
			if (OnGridTileHoveredStop.IsBound())
			{
				OnGridTileHoveredStop.Broadcast(GridTileHovered);
			}
			
			GridTileHovered->SetHovered(false);
			GridTileHovered = nullptr;
		}
		
		GridTileHovered = GridTile;
		GridTileHovered->SetHovered(true);
		if (OnGridTileHoveredStart.IsBound())
		{
			OnGridTileHoveredStart.Broadcast(GridTileHovered);
		}

		// update unit
		if (IsValid(GridUnitHovered))
		{
			if (OnGridUnitHoveredStop.IsBound())
			{
				OnGridUnitHoveredStop.Broadcast(GridUnitHovered);
			}
			
			GridUnitHovered->SetHovered(false);
			GridUnitHovered = nullptr;			
		}
		
		const FGridPosition GridPosition = UGridHelper::CalculateGridPosition(GridTileHovered);
		if (LocationGridUnitMap.Contains(GridPosition))
		{
			GridUnitHovered = LocationGridUnitMap[GridPosition];
			GridUnitHovered->SetHovered(true);
			if (OnGridUnitHoveredStart.IsBound())
			{
				OnGridUnitHoveredStart.Broadcast(GridUnitHovered);
			}
		}
	}
}

void UGridWorldSubsystem::UpdateTileMapping(AGridTile* GridTile)
{
}

void UGridWorldSubsystem::UpdateTileMappingsAll()
{
}

void UGridWorldSubsystem::UpdateUnitMapping(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }
	
	// remove mapping for grid unit
	LocationGridUnitMap.Remove(GridUnitLocationMap[GridUnit]);
	GridUnitLocationMap.Remove(GridUnit);

	// update the unit that has moved	
	const FGridPosition GridPosition = UGridHelper::CalculateGridPosition(GridUnit);
	LocationGridUnitMap.Add(GridPosition, GridUnit);
	GridUnitLocationMap.Add(GridUnit, GridPosition);
}

void UGridWorldSubsystem::UpdateUnitMappingsAll()
{
	for (AGridUnit* GridUnit : GridUnitsAll)
	{
		UpdateUnitMapping(GridUnit);
	}
}

AGridUnit* UGridWorldSubsystem::GetGridUnitOnTile(const AGridTile* GridTile) const
{
	if (GridTileLocationMap.Contains(GridTile))
	{
		if (LocationGridUnitMap.Contains(GridTileLocationMap[GridTile]))
		{
			return LocationGridUnitMap[GridTileLocationMap[GridTile]];
		}
	}

	return nullptr;
}

AGridTile* UGridWorldSubsystem::GetGridTileOfUnit(const AGridUnit* GridUnit) const
{
	if (GridUnitLocationMap.Contains(GridUnit))
	{
		if (LocationGridTileMap.Contains(GridUnitLocationMap[GridUnit]))
		{
			return LocationGridTileMap[GridUnitLocationMap[GridUnit]];
		}
	}
		
	return nullptr;
}

TArray<AGridTile*> UGridWorldSubsystem::GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range)
{
	TArray<AGridTile*> Output;
	TArray<FGridPosition> Temp;
	UGridHelper::GetGridPositionsAtRange(StartGridPosition, Range, Temp);
	for (FGridPosition GridLocation : Temp)
	{
		if (LocationGridTileMap.Contains(GridLocation))
		{
			Output.Add(LocationGridTileMap[GridLocation]);
		}
	}

	return Output;
}

TArray<AGridTile*> UGridWorldSubsystem::GetGridTilesAtRanges(
	const FGridPosition StartGridPosition, TArray<int32> Ranges)
{
	TSet UniqueRanges(Ranges);
	for (const int32 Range : Ranges)
	{
		UniqueRanges.Add(FMath::Abs(Range));
	}

	TSet<AGridTile*> Output;
	for (const int32 Range : UniqueRanges)
	{
		Output.Append(GetGridTilesAtRange(StartGridPosition, Range));
	}

	return Output.Array();
}

void UGridWorldSubsystem::CalculateGridMovement(TArray<FGridMovement>& OutMovement, const AGridUnit* GridUnit, const int32 AvailableMovement)
{
	if (!GridUnitLocationMap.Contains(GridUnit))
	{
		// GridUnit does not have a location...
		UE_LOG(LogTemp, Error, TEXT("Grid unit does not have location!!"));
		return;
	}
	
	AGridTile* StartTile = LocationGridTileMap[GridUnitLocationMap[GridUnit]];    
	if (!IsValid(StartTile))
	{
		// UE_LOG(LogTemp, Error, TEXT("Tile Actor At Location (X:%d, Y:%d) is null!!"), StartGridPosition.X, StartGridPosition.Y);
		UE_LOG(LogTemp, Error, TEXT("No grid tile at unit location!!"));
		return;
	}

	// for now just make sure it is clear, might want to add boolean to check to do this...
	OutMovement.Empty();
 
	// ALGORITHM
	TSet<FGridMovement> Processed;
	TQueue<FGridMovement> ProcessingQueue;
	
	FGridMovement StartNode;
	StartNode.Cost = 0;
	StartNode.GridTile = StartTile;    	
	ProcessingQueue.Enqueue(StartNode);
 
	FGridMovement Current;
	while (!ProcessingQueue.IsEmpty())
	{		
		ProcessingQueue.Dequeue(Current);
		Processed.Add(Current);
    		
		for (AGridTile* TargetTile : GetGridTilesAtRange(GridTileLocationMap[Current.GridTile], 1))
		{
			if (GetGridUnitOnTile(TargetTile)) { continue; }    	
			const int32 CalculatedMovement = Current.Cost + TargetTile->GetMovementCost(); 
			// if (GridUnit->GetAvailableMovement() < CalculatedMovement)
			if (AvailableMovement < CalculatedMovement)
			{
				continue;
			}
    			
			FGridMovement Temp;
			Temp.GridTile = TargetTile;
			Temp.Cost = CalculatedMovement;
			Temp.ParentTile = Current.GridTile;

			if (!Processed.Contains(Temp))
			{
				ProcessingQueue.Enqueue(Temp);
			}
			else
			{
				FGridMovement* ProcessedCopy = Processed.Find(Temp);
				if (Temp.Cost < ProcessedCopy->Cost)
				{
					Processed.Remove(*ProcessedCopy);
					ProcessingQueue.Enqueue(Temp);
				}
			}	
		}		
	}
	
	OutMovement = Processed.Array();
}

void UGridWorldSubsystem::CalculateGridAttacks(
	TArray<FGridPair>& OutGridPairs, AGridUnit* GridUnit, const TArray<FGridMovement>& InGridMovements)
{
	if (!IsValid(GridUnit)) { return; }

	// get enemy info
	TArray<AGridUnit*> EnemyGridUnits;
	if (UTurnWorldSubsystem* TurnSubsystem = GetWorld()->GetSubsystem<UTurnWorldSubsystem>())
	{
		TurnSubsystem->GetFactionEnemies(GridUnit, EnemyGridUnits);
	}
	TArray<FGridPosition> EnemyPositions;
	for (auto Unit : EnemyGridUnits)
	{
		EnemyPositions.Add(UGridHelper::CalculateGridPosition(Unit));
	}

	// unit info
	TMap<int32, TSet<AGridTile*>> RangeTileMap;
	TSet<int32> WeaponRanges;
	// TODO: FOR NOW
	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	if (!GameMode) { return; }
	for (auto Weapon : GridUnit->WeaponsInventory)
	{
		WeaponRanges.Add(GameMode->GetCombatCalculator()->GetWeaponRange(Weapon));
	}
	TMap<FGridPosition, bool> MovementMap;
	for (const auto GridMovement : InGridMovements)
	{
		MovementMap.Add(GridTileLocationMap[GridMovement.GridTile], false);
	}

	TArray<FGridPosition> TempPositions;
	for (AGridUnit* EnemyUnit : EnemyGridUnits)
	{
		for (const int32 WeaponRange : WeaponRanges)
		{
			TempPositions.Empty();
			UGridHelper::GetGridPositionsAtRange(
				UGridHelper::CalculateGridPosition(EnemyUnit), WeaponRange, TempPositions);
			for (FGridPosition RangePosition : TempPositions)
			{
				if (MovementMap.Contains(RangePosition) && MovementMap[RangePosition] == false)
				{
					MovementMap[RangePosition] = true;
					// find the tile that matches with this unit					
					OutGridPairs.Add(
						FGridPair(LocationGridTileMap[GridUnitLocationMap[EnemyUnit]], EnemyUnit));
				}
			}
		}		
	}
}

// calculate attack tiles for a single enemy unit
// TODO: we could make this more robust and have specific rules around weapon types and ranges, etc.
void UGridWorldSubsystem::CalculateGridAttackTiles(TMap<AGridTile*, int32>& OutWeaponPositions,
	const TArray<FGridMovement>& InGridMovements, const AGridUnit* InstigatorUnit, const AGridUnit* TargetUnit)
{
	if (!IsValid(InstigatorUnit) || !IsValid(TargetUnit)) { return; }

	AGameMode_TurnBased_Combat* GameMode = Cast<AGameMode_TurnBased_Combat>(GetWorld()->GetAuthGameMode());
	if (!GameMode) { return; }
	const UCombatCalculator_Basic* CombatCalculator = GameMode->GetCombatCalculator();
	TSet<int32> WeaponRanges;
	for (auto Weapon : InstigatorUnit->WeaponsInventory)
	{
		WeaponRanges.Add(CombatCalculator->GetWeaponRange(Weapon));
	}
	// AGridTile* TargetTile = GetGridTileOfUnit(TargetUnit);
	for (auto WeaponRange : WeaponRanges)
	{
		TArray<FGridPosition> OutWeaponRangePositions;
		UGridHelper::GetGridPositionsAtRange(UGridHelper::CalculateGridPosition(TargetUnit), WeaponRange, OutWeaponRangePositions);
		for (const FGridPosition GridPosition : OutWeaponRangePositions)
		{
			// don't add tile of instigator
			if (GridPosition == UGridHelper::CalculateGridPosition(GetGridTileOfUnit(InstigatorUnit)))
			{
				continue;
			}
			
			for (auto GridMovement : InGridMovements)
			{
				if (GridTileLocationMap[GridMovement.GridTile] == GridPosition)
				{
					OutWeaponPositions.Add(LocationGridTileMap[GridPosition], WeaponRange);
				}
			}
		}
	}
	
}


void UGridWorldSubsystem::OnGridUnitAbilityActivated(UGameplayAbility* InGameplayAbility)
{
	// not current actions being taken therefore fire event
	AGridUnit* InGridUnit = Cast<AGridUnit>(InGameplayAbility->GetAvatarActorFromActorInfo());
	if (!IsValid(InGridUnit)) { return; }
	
	if (GridUnitsTakingActions.IsEmpty())
	{
		if (OnGridUnitAbilityStart.IsBound()) { OnGridUnitAbilityStart.Broadcast(); }
	}
	
	TPair<AGridUnit*, UGameplayAbility*> AbilityPair;
	AbilityPair.Key = InGridUnit;
	AbilityPair.Value = InGameplayAbility;
	GridUnitsTakingActions.Add(AbilityPair);

	UE_LOG(LogTemp, Log, TEXT("On Grid Unit Ability Activated: %d"), GridUnitsTakingActions.Num());

}

void UGridWorldSubsystem::OnGridUnitAbilityEnded(UGameplayAbility* InGameplayAbility)
{
	AGridUnit* InGridUnit = Cast<AGridUnit>(InGameplayAbility->GetAvatarActorFromActorInfo());
	if (!IsValid(InGridUnit)) { return; }
	
	TPair<AActor*, UGameplayAbility*> FoundPair;
	for (TPair<AActor*, UGameplayAbility*> Pair : GridUnitsTakingActions)
	{
		if (Pair.Key == InGridUnit && Pair.Value == InGameplayAbility)
		{
			FoundPair = Pair;
			break;
		}
	}
	
	if (FoundPair.Key == InGridUnit && FoundPair.Value == InGameplayAbility)
	{
		GridUnitsTakingActions.Remove(FoundPair);

		// if this array is empty then no units are taking actions and it is safe to return PC control
		if (GridUnitsTakingActions.IsEmpty())
		{
			UpdateUnitMappingsAll();
			if (OnGridUnitAbilityEnd.IsBound()) { OnGridUnitAbilityEnd.Broadcast(); }

			UE_LOG(LogTemp, Log, TEXT("On Grid Unit Ability Ended: no more units taking actions"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("On Grid Unit Ability Ended: %d"), GridUnitsTakingActions.Num());
	}
}

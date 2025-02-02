// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridWorldSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Grid/GridHelper.h"
#include "Tile/GridTile.h"
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

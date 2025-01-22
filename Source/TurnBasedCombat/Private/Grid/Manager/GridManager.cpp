// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Manager/GridManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
// #include "Grid/GridMovementNode.h"
#include "Combat/CombatCalculator/MoveAbility.h"
#include "Combat/CombatCalculator/MoveAbilityPayload.h"
#include "Grid/GridUtility.h"
#include "Grid/GridStructs.h"
#include "Grid/Event/GridEventPayload_Move.h"
#include "Grid/Manager/TurnManager.h"
#include "Grid/Manager/GridEventPayload.h"
#include "Grid/Manager/GridRules.h"
#include "TurnBasedCombat/Public/Grid/Tile/GridTile.h"
#include "TurnBasedCombat/Public/Grid/Unit/GridUnit.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Grid_Move, "Event.Grid.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Grid_Attack, "Event.Grid.Attack");


UGridManager::UGridManager()
{	
}

void UGridManager::Initialize(UGridRules* InGridRules, UTurnManager* InTurnManager)
{
	GridRules = InGridRules;
	TurnManager = InTurnManager;
	// TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
	//
	// // bind function to FactionWin
	// TurnManager->OnFactionWin.BindUObject(this, &ThisClass::)
}

void UGridManager::RegisterGridTile(AGridTile* GridTile)
{
	if (IsValid(GridTile) && !GridTilesAll.Contains(GridTile))
	{
		GridTilesAll.AddUnique(GridTile);
		
		// add to map
		FGridPosition GridPosition = UGridUtility::CalculateGridPosition(GridTile);
		LocationGridTileMap.Add(GridPosition, GridTile);
		GridTileLocationMap.Add(GridTile, GridPosition);
		
		// bind to events
		GridTile->OnBeginCursorOver.AddDynamic(this, &UGridManager::OnBeginCursorOverGridTile);
	}
}

void UGridManager::RegisterGridUnit(AGridUnit* GridUnit)
{
	if (IsValid(GridUnit) && !GridUnitsAll.Contains(GridUnit))
	{
		GridUnitsAll.AddUnique(GridUnit);
		
		// add to map
		FGridPosition GridPosition = UGridUtility::CalculateGridPosition(GridUnit);
		
		// TODO: why the double map???
		LocationGridUnitMap.Add(GridPosition, GridUnit);
		GridUnitLocationMap.Add(GridUnit, GridPosition);
		
		// add to turn manager
		TurnManager->RegisterGridUnit(GridUnit);
		
		GridUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddLambda([this, GridUnit](const FAbilityEndedData& Data)
		{
			if (Data.AbilitySpecHandle == GridUnit->GameplayAbilitySpecHandle_Move)
			{
				PostEvent_Move(GridUnit);
			}
		});
	}
}

UGridProxy* UGridManager::GetCurrentHoveredGridTile()
{
	// check that current hovered tile and current hovered unit exist???
	// NO a proxy is a wrapper for a grid position that will contain a tile and maybe a unit
	if (CurrentHoveredTile == nullptr)
	{
		return nullptr;
	}
	
	// TODO: not finished, need to determine if this makes sense...
	// put together a grid proxy
	const FCalculateGridMovement CalculateGridMovementDelegate;
	TArray<FGridMovement> GridMovements;
	// TArray<const AGridUnit*> OutEnemyUnitsInRange;
	TArray<FGridPair> GridPairs;
	if (CurrentHoveredUnit !=  nullptr)
	{
		CalculateGridMovement(GridMovements, CurrentHoveredUnit);
		// CalculateGridAttacks(OutEnemyUnitsInRange, CurrentHoveredUnit);
		GridPairs = CalculateGridAttacks(CurrentHoveredUnit);
	}
	
	return UGridProxy::CreateGridProxy(
		this,
		TurnManager,
		CurrentHoveredTile,
		CurrentHoveredUnit,
		CalculateGridMovementDelegate,
		GridMovements,
		GridPairs);
	
	// NOTE: cannot create grid proxy with new object as we made the constructor private
	// return NewObject<UGridProxy>(this);
}

UGridProxy* UGridManager::GetNextGridUnit(UGridProxy* InGridProxy, bool Next)
{
	// TODO: if given a proxy with a unit, return next or previous proxy with unit, otherwise return random
	return nullptr;
}

void UGridManager::CreateMoveEvent(UGridProxy* Instigator, UGridProxy* Location)
{
	if (!IsValid(Instigator) || !IsValid(Location))
	{
		return;
	}

	// check if this unit can move and location does not contain a unit
	if (!TurnManager->CanTakeTurn(Instigator->GridUnit) || Location->GridUnit != nullptr)
	{
		return;
	}
	
	// broadcast event starting
	if (OnGridEventStart.IsBound())
	{
		OnGridEventStart.Broadcast();
	}

	// TODO: will this just keep adding the same lambda over and over???
	// ATTACHING TO THIS CALLBACK WHEN REGISTERING WITH GRIDMANAGER
	// Instigator->GridUnit->GetAbilitySystemComponent()->OnAbilityEnded.AddLambda([this, Instigator](const FAbilityEndedData& Data)
	// {
	// 	if (Data.AbilitySpecHandle == Instigator->GridUnit->MoveAbilitySpecHandle)
	// 	{
	// 		PostEvent_Move(Instigator->GridUnit);
	// 	}
	// });

	// DOES NOT WORK
	// update tiles on ability...
	// FGameplayAbilitySpec* Spec = Instigator->GridUnit->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(
	// 	Instigator->GridUnit->MoveAbilitySpecHandle);
	// if (UMoveAbility* MoveAbility = Cast<UMoveAbility>(Spec->Ability))
	// {
	// 	TArray<AGridTile*> Tiles;
	// 	Tiles.Add(Location->GridTile);
	// 	MoveAbility->SetMovementTiles(Tiles);
	// 	// activate ability...
	// 	// Instigator->GridUnit->AbilitySystemComponent->TryActivateAbility(Instigator->GridUnit->MoveAbilitySpecHandle);
	//
	// }

	/*
	 * LEARNING
	 * seems the only good way to pass information to a gameplay ability is through the event data
	 * this can be helpful as you will have to cast the optional data an if the cast fails then it will
	 * prevent abilities from firing with incorrect data...
	 */
	FGameplayEventData EventData;
	EventData.Instigator = Instigator->GridUnit;
	TArray<AGridTile*> Tiles;
	Tiles.Add(Location->GridTile);
	EventData.OptionalObject = UMoveAbilityPayload::Create(Tiles);

	// send gameplay event
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Instigator->GridUnit, TAG_Event_Grid_Move, EventData);

	// activate ability...
	// Instigator->GridUnit->AbilitySystemComponent->TryActivateAbility(Instigator->GridUnit->MoveAbilitySpecHandle);

	// // subscribe to end event and notify grid unit to move
	// Instigator->GridUnit->OnEventMoveEnd.AddUniqueDynamic(this, &ThisClass::PostEvent_Move);
	// // TODO: use a gameplay event instead
	// Instigator->GridUnit->MovementEvent(Location->GridTile->GetPlacementLocation());
}

void UGridManager::PostEvent_Move(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }

	// update turn manager
	TurnManager->UpdateGridUnitActionTaken(GridUnit);
	
	// update unit mappings
	// remove mapping for grid unit (will be updated on complete)
	LocationGridUnitMap.Remove(GridUnitLocationMap[GridUnit]);
	GridUnitLocationMap.Remove(GridUnit);

	// update the unit that has moved	
	const FGridPosition GridPosition = UGridUtility::CalculateGridPosition(GridUnit);
	LocationGridUnitMap.Add(GridPosition, GridUnit);
	GridUnitLocationMap.Add(GridUnit, GridPosition);
		
	if (OnGridEventEnd.IsBound())
	{
		OnGridEventEnd.Broadcast();
	}
}

void UGridManager::CreateAttackEvent(UGridProxy* Instigator, UGridProxy* Target, UGridProxy* Location)
{
	// TODO
	if (!IsValid(Instigator) || !IsValid(Location) || !IsValid(Target))
	{
		UE_LOG(LogTemp, Error, TEXT("Create Attack Event: parameter null..."));
		return;
	}

	// check if this unit can move
	// check if location is empty or is instigator (instigator does not move)
	// check target has a grid unit
	if (!TurnManager->CanTakeTurn(Instigator->GridUnit) ||
		(Location->GridUnit != nullptr && Location->GridUnit != Instigator->GridUnit) ||
		Target->GridUnit == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Create Attack Event: conditions not met..."));
		return;
	}
	
	// broadcast event starting
	if (OnGridEventStart.IsBound())
	{
		OnGridEventStart.Broadcast();
	}
	
	// subscribe to end event and notify grid unit to move
	Instigator->GridUnit->OnEventAttackEnd.AddUniqueDynamic(this, &ThisClass::PostEvent_Attack);

	// TODO: use a gameplay event instead
	// Instigator->GridUnit->AttackEvent(Location->GridTile->GetPlacementLocation(), Target->GridUnit);

	// TODO: try and activate through gameplay event instead of this current way...
	// create gameplay event information
	// UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator->GridUnit);
	// FGameplayAbilitySpecHandle AbilityHandle;
	// FGameplayAbilityActorInfo ActorInfo = *AbilitySystemComponent->AbilityActorInfo.Get();
	FGameplayEventData EventData;
	EventData.Instigator = Instigator->GridUnit;
	EventData.Target = Target->GridUnit;
	UGridEventPayload_Move* GridEventPayload = UGridEventPayload_Move::CreatePayload(Location->GridTile->GetPlacementLocation());
	EventData.OptionalObject = GridEventPayload;

	// send gameplay event
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Instigator->GridUnit, TAG_Event_Grid_Attack, EventData);
	// AbilitySystemComponent->TriggerAbilityFromGameplayEvent(
	// 	AbilityHandle,
	// 	&ActorInfo,
	// 	TAG_Event_Grid_Attack,
	// 	&EventData, 
	// 	*AbilitySystemComponent);
}

void UGridManager::PostEvent_Attack(AGridUnit* GridUnit)
{
	if (!IsValid(GridUnit)) { return; }

	// update turn manager
	TurnManager->UpdateGridUnitActionTaken(GridUnit);
	
	// update unit mappings
	// remove mapping for grid unit (will be updated on complete)
	LocationGridUnitMap.Remove(GridUnitLocationMap[GridUnit]);
	GridUnitLocationMap.Remove(GridUnit);

	// update the unit that has moved	
	const FGridPosition GridPosition = UGridUtility::CalculateGridPosition(GridUnit);
	LocationGridUnitMap.Add(GridPosition, GridUnit);
	GridUnitLocationMap.Add(GridUnit, GridPosition);
		
	if (OnGridEventEnd.IsBound())
	{
		OnGridEventEnd.Broadcast();
	}
}

void UGridManager::OnEndEvent()
{
	if (OnGridEventEnd.IsBound())
	{
		OnGridEventEnd.Broadcast();
	}	
}

bool UGridManager::IsMatch(const UGridProxy* GridProxy_A, const UGridProxy* GridProxy_B)
{
	bool bMatch_Tile = GridProxy_A->GridTile == GridProxy_B->GridTile;
	bool bMatch_Unit = GridProxy_A->GridUnit == GridProxy_B->GridUnit;
	return bMatch_Tile && bMatch_Unit;
}

void UGridManager::OnBeginCursorOverGridTile(AActor* Actor)
{
	if (AGridTile* GridTile = Cast<AGridTile>(Actor); IsValid(GridTile))
	{		
		// tile
		if (IsValid(CurrentHoveredTile))
		{
			CurrentHoveredTile->SetHovered(false);
		}
		
		CurrentHoveredTile = GridTile;
		CurrentHoveredTile->SetHovered(true);
		if (OnGridTileHovered.IsBound())
		{
			OnGridTileHovered.Broadcast(CurrentHoveredTile);
		}

		// unit
		if (CurrentHoveredUnit != nullptr)
		{
			CurrentHoveredUnit->SetHovered(false);
			CurrentHoveredUnit = nullptr;
		}

		// TODO: simplify this with better tile/unit setup or grid proxy
		const FGridPosition GridPosition = UGridUtility::CalculateGridPosition(CurrentHoveredTile);
		if (LocationGridUnitMap.Contains(GridPosition))
		{
			CurrentHoveredUnit = LocationGridUnitMap[GridPosition];
			CurrentHoveredUnit->SetHovered(true);
			if (OnGridUnitHovered.IsBound())
			{
				OnGridUnitHovered.Broadcast(CurrentHoveredUnit);
			}
		}
	}
}

void UGridManager::CalculateGridMovement(TArray<FGridMovement>& OutMovement, AGridUnit* GridUnit)
{
	// ERROR CHECKS    
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
			if (!GridRules->UnitCanMoveOnTile(this, GridUnit, Current.GridTile, TargetTile))
			{
				continue;
			}    				
    			
			const int32 TerrainCost = GridRules->CalculateTerrainCost(this, GridUnit, Current.GridTile, TargetTile);
			const int32 CalculatedMovement = Current.Cost + TerrainCost; 
			if (GridUnit->GetAvailableMovement() < CalculatedMovement)
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

#pragma region testing

	// ERROR CHECK
	// get start tile from terrain, make FStruct and set values, add Struct to queue

	// NEW ALGORITHM
	// TMap<AGridTile*, AGridTile*> ParentTiles;
	// TMap<AGridTile*, int32> ProcessedTiles;
	// TQueue<AGridTile*> TilesToProcess;
	//
	// TilesToProcess.Enqueue(LocationGridTileMap[GridUnitLocationMap[GridUnit]]);
	//
	// AGridTile* CurrentTile;
	// while(!TilesToProcess.IsEmpty())
	// {
	// 	TilesToProcess.Dequeue(CurrentTile);
	// 	for (AGridTile* GridTile : GetGridTilesAtRange(UGridUtility::CalculateGridPosition(CurrentTile), 1))
	// 	{
	// 		AGridUnit* OccupyingGridUnit = LocationGridUnitMap[GridTileLocationMap[GridTile]];
	// 		if (GridRules->UnitCanMoveOnTile(GridUnit, GridTile, OccupyingGridUnit))
	// 		{
	// 			const int32 TerrianCost = GridRules->CalculateTerrainCost(GridUnit, GridTile, OccupyingGridUnit);
	// 			const int32 CalculatedMovement = GridUnit->GetAvailableMovement(); - TerrianCost;
 //    				
	// 			if (CalculatedMovement >= 0)
	// 			{}
	// 		}
	// 	}
	//
	// 	ProcessedTiles.Add(CurrentTile);
	// }
	
	// ALGORITHM
	// check for null errors
	// if (!GridUnitLocationMap.Contains(GridUnit))
	// {
	// 	// GridUnit does not have a location...
	// 	UE_LOG(LogTemp, Error, TEXT("Grid unit does not have location!!"));
	// 	return TArray<FGridMovement>();
	// }
	//
	// // const AGridTile* StartTile = *GridTileMap.Find(StartGridPosition);
	// AGridTile* StartTile = LocationGridTileMap[GridUnitLocationMap[GridUnit]];
 //    
	// if (!IsValid(StartTile))
	// {
	// 	// UE_LOG(LogTemp, Error, TEXT("Tile Actor At Location (X:%d, Y:%d) is null!!"), StartGridPosition.X, StartGridPosition.Y);
	// 	UE_LOG(LogTemp, Error, TEXT("No grid tile at unit location!!"));
	// 	return TArray<FGridMovement>();
	// }
 //
	// // setup containers for logic
	// TArray<UGridMovementNode*> Explored;
	// // TArray<UGridMovementNode*> ToProcess;
	// TQueue<UGridMovementNode*> Unexplored;
	//
 //    UGridMovementNode* StartNode = NewObject<UGridMovementNode>();
	// StartNode->GridTile = StartTile;
	// Unexplored.Enqueue(StartNode);
 //
 //    UGridMovementNode* Current;
 //    while(!Unexplored.IsEmpty())
 //    {
 //    	Unexplored.Dequeue(Current);
	//     // check if already in processed
 //    	bool bProcess = true;
 //    	if (Explored.Contains(Current))
 //    	{
 //    		UGridMovementNode* Matching = Explored[Explored.Find(Current)];
 //    		// if processed has a higher value then need to reprocess so remove, else skip this one
 //    		if (Current->CostToReachTile < Matching->CostToReachTile)
 //    		{
 //    			Explored.Remove(Matching);
 //    		}
 //    		else
 //    		{
 //    			bProcess = false;
 //    		}
 //    	}
 //
 //    	if (!bProcess) { continue; }
 //    		
 //    	for (AGridTile* TargetTile : GetGridTilesAtRange(GridTileLocationMap[Current->GridTile], 1))
 //    	{
 //    		if (GridRules->UnitCanMoveOnTile(this, GridUnit, Current->GridTile, TargetTile))
 //    		{
 //    			const int32 TerrainCost = GridRules->CalculateTerrainCost(this, GridUnit, Current->GridTile, TargetTile);
 //    			const int32 CalculatedMovement = Current->CostToReachTile - TerrainCost;
 //    				
 //    			if (CalculatedMovement >= 0)
 //    			{
 //    				UGridMovementNode* Temp = NewObject<UGridMovementNode>();
 //    				Temp->GridTile = TargetTile;
 //    				Temp->CostToReachTile = CalculatedMovement;
 //    				Temp->ParentNode = Current;
 //
 //    				ProcessingQueue.Enqueue(Temp);
 //    			}
 //    		}
 //    	}
 //
 //    	Processed.Add(Current);
 //    }
 //
	// // // why does this matter???
	//  //    // convert to array and sort according to gridlocation
	//  //    TArray<FGridMovement> Output = Processed.Array();
	//  //    Output.Sort([](const FGridMovement& A, const FGridMovement& B)
	//  //    {
	//  //    	return A.GridPosition.X < B.GridPosition.X ? true : A.GridPosition.Y < B.GridPosition.Y ? true : false;
	//  //    });
 //
	// // go through movement nodes and remove any nodes that have a unit on them
	// // even if units can move through other units they still cannot finish on the same tile as another unit
	// // in general this is the way TB games work, if there is a need to change this later then so be it...
	// TArray<UGridMovementNode*> Output;
	// for (auto Node : Processed)
	// {
	// 	if (!GetGridUnitOnTile(Node->GridTile))
	// 	{
	// 		Output.Add(Node);
	// 	}
	// }
 //    
 //    return Output;
#pragma endregion
}

void UGridManager::CalculateGridAttacks(TArray<const AGridUnit*> OutGridUnitsInRange, AGridUnit* GridUnit)
{	
	TMap<int32, TSet<AGridTile*>> RangeTileMap;
	TSet<int32> WeaponRanges = GridUnit->GetWeaponRanges();
	TArray<FGridPosition> EnemyPositions = GetEnemyPositions(GridUnit);
	TArray<AGridUnit*> EnemyGridUnits;
	GetEnemyUnits(EnemyGridUnits, GridUnit);
	TArray<FGridMovement> GridMovements;
	CalculateGridMovement(GridMovements, GridUnit);
	TMap<FGridPosition, bool> MovementMap;
	for (const auto GridMovement : GridMovements)
	{
		MovementMap.Add(GridTileLocationMap[GridMovement.GridTile], false);
	}

	TArray<FGridPosition> TempPositions;
	for (const AGridUnit* EnemyUnit : EnemyGridUnits)
	{
		for (const int32 WeaponRange : WeaponRanges)
		{
			TempPositions.Empty();
			UGridUtility::GetGridPositionsAtRange(
				UGridUtility::CalculateGridPosition(EnemyUnit), WeaponRange, TempPositions);
			for (FGridPosition RangePosition : TempPositions)
			{
				if (MovementMap.Contains(RangePosition) && MovementMap[RangePosition] == false)
				{
					MovementMap[RangePosition] = true;
					OutGridUnitsInRange.AddUnique(EnemyUnit);
				}
			}
		}		
	}
}

TArray<FGridPair> UGridManager::CalculateGridAttacks(AGridUnit* GridUnit)
{
	TArray<FGridPair> OutGridPairs;

	TMap<int32, TSet<AGridTile*>> RangeTileMap;
	TSet<int32> WeaponRanges = GridUnit->GetWeaponRanges();
	TArray<FGridPosition> EnemyPositions = GetEnemyPositions(GridUnit);
	TArray<AGridUnit*> EnemyGridUnits;
	GetEnemyUnits(EnemyGridUnits, GridUnit);
	TArray<FGridMovement> GridMovements;
	CalculateGridMovement(GridMovements, GridUnit);
	TMap<FGridPosition, bool> MovementMap;
	for (const auto GridMovement : GridMovements)
	{
		MovementMap.Add(GridTileLocationMap[GridMovement.GridTile], false);
	}

	TArray<FGridPosition> TempPositions;
	for (AGridUnit* EnemyUnit : EnemyGridUnits)
	{
		for (const int32 WeaponRange : WeaponRanges)
		{
			TempPositions.Empty();
			UGridUtility::GetGridPositionsAtRange(
				UGridUtility::CalculateGridPosition(EnemyUnit), WeaponRange, TempPositions);
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
	
	return OutGridPairs;
}

TArray<FTargetingUnit> UGridManager::CalculateGridTargets(AGridUnit* GridUnit)
{
	// first round filter
		// find valid moveable tiles that can target enemies
		// for each valid tile, 




	//////////////////////////
	TArray<FTargetingUnit> UnitsBeingTargeted;
	
	TMap<int32, TSet<AGridTile*>> RangeTileMap;
	TSet<int32> WeaponRanges = GridUnit->GetWeaponRanges();
	TArray<FGridPosition> EnemyPositions = GetEnemyPositions(GridUnit);
	TArray<FGridMovement> GridMovements;
	CalculateGridMovement(GridMovements, GridUnit);
	TMap<FGridPosition, bool> MovementMap;
	// TMap<FGridPosition, TArray<int32>> SuccessMap;
	for (const auto GridMovement : GridMovements)
	{
		MovementMap.Add(GridTileLocationMap[GridMovement.GridTile], false);
	}

	TArray<FGridPosition> TempPositions;
	for (const FGridPosition EnemyPosition : EnemyPositions)
	{
		for (const int32 WeaponRange : WeaponRanges)
		{
			UGridUtility::GetGridPositionsAtRange(EnemyPosition, WeaponRange, TempPositions);
			for (FGridPosition RangePosition : TempPositions)
			{
				if (MovementMap.Contains(RangePosition))
				{
					MovementMap[RangePosition] = true;
					// if (!SuccessMap.Contains(RangePosition))
					// {
					// 	SuccessMap.Add(RangePosition, TArray<int32>());						
					// }
					//
					// SuccessMap[RangePosition].AddUnique(WeaponRange);
				}
			}
		}
		
	}

	return TArray<FTargetingUnit>();



	///////////////////////////////////////////////////////////
	// GWS->GetUnitGridMovement(ActiveUnit, SelectedUnitGridMovement);

	// only test against tiles that have units not on the same team
	// auto GridTilesWithUnits = GWS->GetGridTilesWithUnitsByTeam(ActiveUnit, false);

	// for (auto GridTileWithUnit : GridTilesWithUnits)
	// {
	// 	FTargetingUnit TargetingUnit;
	// 	TargetingUnit.GridTile = GridTileWithUnit;
	// 	
	// 	auto TempGridPosition = GWS->CalculateGridPosition(GridTileWithUnit);
	// 	for (auto Range : WeaponRanges)
	// 	{
	// 		for (auto GridTileAtRange : GWS->GetGridTilesAtRange(TempGridPosition, Range))
	// 		{
	// 			for (auto UnitMovementTile : SelectedUnitGridMovement)
	// 			{
	// 				if (UnitMovementTile.GridTile == GridTileAtRange)
	// 				{
	// 					if (TargetingUnit.RangeMap.Contains(Range))
	// 					{
	// 						TargetingUnit.RangeMap[Range].AddUnique(UnitMovementTile.GridTile);
	// 					}
	// 					else
	// 					{
	// 						TargetingUnit.RangeMap.Add(Range, TArray<TSoftObjectPtr<AATBGridTile>>());
	// 						TargetingUnit.RangeMap[Range].AddUnique(UnitMovementTile.GridTile);
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	//
	// 	// only add if there are tiles in range
	// 	if (!TargetingUnit.RangeMap.IsEmpty())
	// 	{
	// 		UnitsBeingTargeted.Add(TargetingUnit);
	// 	}
	// }
	//
	// // highlight all attackable units
	// for (auto TargetingUnit : UnitsBeingTargeted)
	// {
	// 	TargetingUnit.GridTile->SetGridTileTagState(TAG_Grid_State_Attackable);
	// }
	//
	// return UnitsBeingTargeted;
}

void UGridManager::GetEnemyUnits(TArray<AGridUnit*>& EnemyGridUnits, AGridUnit* GridUnit)
{
	// TODO: will need to have some checking to not include allied factions...
	for (const auto Unit : GridUnitsAll)
	{
		if (GridUnit->Execute_GetFaction(GridUnit) != Unit->Execute_GetFaction(Unit))
		{
			EnemyGridUnits.AddUnique(Unit);
		}		
	}
}

TArray<FGridPosition> UGridManager::GetEnemyPositions(const AGridUnit* GridUnit) const
{
	TArray<FGridPosition> Positions;
	for (const auto Unit : GridUnitsAll)
	{
		if (GridUnit->Execute_GetFaction(GridUnit) != Unit->Execute_GetFaction(Unit))
		{
			Positions.AddUnique(UGridUtility::CalculateGridPosition(Unit));
		}		
	}
	
	return Positions;
}

TArray<AGridTile*> UGridManager::GetGridTilesAtRange(FGridPosition StartGridPosition, int32 Range)
{
	TArray<AGridTile*> Output;
	TArray<FGridPosition> Temp;
	UGridUtility::GetGridPositionsAtRange(StartGridPosition, Range, Temp);
	for (FGridPosition GridLocation : Temp)
	{
		if (LocationGridTileMap.Contains(GridLocation))
		{
			Output.Add(LocationGridTileMap[GridLocation]);
		}
	}

	return Output;
}

TArray<AGridTile*> UGridManager::GetGridTilesAtRanges(const FGridPosition StartGridPosition, TArray<int32> Ranges)
{
	// TODO: this look funny???
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

TArray<AGridTile*> UGridManager::GetGridTilesInRange(const FGridPosition StartGridPosition, int32 Range)
{
	Range = FMath::Abs(Range);
	TSet<FGridPosition> GridLocationsSet;
	TArray<FGridPosition> NewGridPositions;

	TSet<AGridTile*> Output;
	for (int32 i = 1; i <= Range; i++)
	{
		Output.Append(GetGridTilesAtRange(StartGridPosition, i));
	}

	return Output.Array();
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Grid/Manager/GridManager.h"
#include "WinCondition_Abstract.h"
#include "TurnBasedCombatGameMode.generated.h"


class UTurnManager;
class UGridProxy;
class UEventSystem;
class UGridManager;
class AGridUnit;
class AGridTile;
class UGridRules;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnBasedCombat_Events);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnBasedCombat_TileHovered, const AGridTile*, GridTile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnBasedCombat_UnitHovered, const AGridUnit*, GridUnit);
// DECLARE_EVENT(ATurnBasedCombatGameMode, FGridEvent);

/**
 * TODO
 * the plan for now is to use the gamemode as a master facade for objects to register
 * and to listen to important delegates to help control the flow of combat
 * therefore there will be a lot of passing functions
 */
UCLASS()
class TURNBASEDCOMBAT_API ATurnBasedCombatGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATurnBasedCombatGameMode();

	virtual void BeginPlay() override;

	UEventSystem* GetEventSystem();

	// GameMode Interface ~ start
	UPROPERTY(BlueprintAssignable)
	FTurnBasedCombat_Events OnCombatEnable;
	UPROPERTY(BlueprintAssignable)
	FTurnBasedCombat_Events OnCombatDisable;
	// GameMode Interface ~ end
	
	// GridManager Interface ~ start
	UPROPERTY(BlueprintAssignable)
	FTurnBasedCombat_Events OnGridEventStart;
	UPROPERTY(BlueprintAssignable)
	FTurnBasedCombat_Events OnGridEventEnd;
	UFUNCTION(BlueprintCallable)
	void RegisterGridTile(AGridTile* GridTile);
	UFUNCTION(BlueprintCallable)
	void RegisterGridUnit(AGridUnit* GridUnit);
	UPROPERTY(BlueprintAssignable)
	FTurnBasedCombat_TileHovered OnGridTileHovered;
	UPROPERTY(BlueprintAssignable)
	FTurnBasedCombat_UnitHovered OnGridUnitHovered;
	// GridManager Interface ~ end

	// Cursor ~ start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
	TObjectPtr<UStaticMesh> CursorMesh;
	UPROPERTY()
	TObjectPtr<AActor> Cursor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
	FVector Cursor_ExtraHeight = FVector(0.f, 0.f, 5.f);
	UFUNCTION()
	void UpdateCursor(const AGridTile* GridTile);
	// Cursor ~ end

protected:
	// win condition ~ start
	UPROPERTY(Instanced, EditDefaultsOnly)
	UWinCondition_Abstract* WinCondition;
	UFUNCTION()
	void OnWinConditionReceived(EWinConditionType InWinCondition);
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnBasedCombatOver, EWinConditionType, ConditionType);
	FTurnBasedCombatOver OnCombatOver;
	// win condition ~ end

	
private:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UPROPERTY()
	UEventSystem* EventSystem = nullptr;
	
	UPROPERTY()
	UGridManager* GridManager = nullptr;

	UPROPERTY()
	UGridRules* GridRules = nullptr;
	
	UPROPERTY()
	UTurnManager* TurnManager = nullptr;
	
};

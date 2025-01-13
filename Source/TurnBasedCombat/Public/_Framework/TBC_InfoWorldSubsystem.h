// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "TBC_InfoWorldSubsystem.generated.h"


class UDuelContainer;
class AGridUnit;
class AGridTile;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridTileChanged, const AGridTile*, GridTile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridUnitChanged, const AGridUnit*, GridUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerControllerChanged, const FGameplayTag, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDuelContainerChanged, const UDuelContainer*, DuelContainer);

/**
 * this will act as a facade for events and accessing important information that designers might need
 */
UCLASS()
class TURNBASEDCOMBAT_API UTBC_InfoWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	// GRID ~
public:
	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FGridTileChanged OnGridTileHovered;
	UFUNCTION()
	void SetGridTileHovered(AGridTile* InGridTile);
	UFUNCTION(BlueprintCallable)
	const AGridTile* GetGridTileHovered() { return GridTileHoveredCurrent; }

	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FGridTileChanged OnGridTileSelected;
	UFUNCTION()
	void SetGridTileSelected(AGridTile* InGridTile);
	UFUNCTION(BlueprintCallable)
	const AGridTile* GetGridTileSelected() { return GridTileSelectedCurrent; }

	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FGridUnitChanged OnGridUnitHovered;
	UFUNCTION()
	void SetGridUnitHovered(AGridUnit* InGridUnit);
	UFUNCTION(BlueprintCallable)
	const AGridUnit* GetGridUnitHovered() { return GridUnitHoveredCurrent; }

	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FGridUnitChanged OnGridUnitSelected;
	UFUNCTION()
	void SetGridUnitSelected(AGridUnit* InGridUnit);
	UFUNCTION(BlueprintCallable)
	const AGridUnit* GetGridUnitSelected() { return GridUnitSelectedCurrent; }
	
private:
	UPROPERTY()
	AGridTile* GridTileHoveredCurrent;
	UPROPERTY()
	AGridTile* GridTileSelectedCurrent;
	UPROPERTY()
	AGridUnit* GridUnitHoveredCurrent;
	UPROPERTY()
	AGridUnit* GridUnitSelectedCurrent;

	// GRID ~ end

	// PC ~
public:
	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FPlayerControllerChanged OnPlayerControllerUpdateMode;
	UFUNCTION()
	void SetPlayerControllerMode(FGameplayTag InGameplayTag);
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetPlayerControllerMode() const { return PlayerControllerMode; }

	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FPlayerControllerChanged OnPlayerControllerUpdatePhase;	
	UFUNCTION()
	void SetPlayerControllerPhase(FGameplayTag InGameplayTag);
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetPlayerControllerPhase() const { return PlayerControllerPhase; }

private:
	UPROPERTY()
	FGameplayTag PlayerControllerMode;
	UPROPERTY()
	FGameplayTag PlayerControllerPhase;	

	// PC ~ end

	// COMBAT ~
public:
	UPROPERTY(BlueprintAssignable, Category="Turn Based Combat")
	FDuelContainerChanged OnDuelContainerChanged;
	UFUNCTION()
	void SetDuelContainer(UDuelContainer* InDuelContainer);
	UFUNCTION(BlueprintCallable)
	UDuelContainer* GetDuelContainer() { return DuelContainerCurrent; }

protected:
	UPROPERTY()
	UDuelContainer* DuelContainerCurrent;

	// COMBAT ~ end
};

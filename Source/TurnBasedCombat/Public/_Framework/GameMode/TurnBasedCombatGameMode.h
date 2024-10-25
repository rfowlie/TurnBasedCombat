// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TurnBasedCombatGameMode.generated.h"


class UEventSystem;
class UGridManager;
class AGridUnit;
class AGridTile;


/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API ATurnBasedCombatGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATurnBasedCombatGameMode();

	virtual void BeginPlay() override;

	UEventSystem* GetEventSystem();
	
	UGridManager* GetGridManager();
	
private:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UPROPERTY()
	UEventSystem* EventSystem;
	
	UPROPERTY()
	UGridManager* GridManager;
};

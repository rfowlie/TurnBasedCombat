// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridStructs.h"
#include "UObject/Object.h"
#include "GridMovementNode.generated.h"

class AGridTile;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UGridMovementNode : public UObject
{
	GENERATED_BODY()

public:
	UGridMovementNode() {}

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTile> GridTile = nullptr;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// FGridPosition GridPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostToReachTile = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGridMovementNode> ParentNode = nullptr;


	// for finding in containers
	bool operator==(const FGridMovement& Other) const
	{
		return GridTile == Other.GridTile;
	}
};

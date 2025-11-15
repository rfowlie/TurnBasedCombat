// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridMechanics_Structs.generated.h"

class AGridTileBase;
class AGridUnitBase;

USTRUCT(BlueprintType)
struct UNREALGRIDMECHANICS_API FGridPosition
{
	GENERATED_BODY()

	FGridPosition() : X(0), Y(0) {}
	FGridPosition(const int32 ValX, const int32 ValY) : X(ValX), Y(ValY) {}
	
	UPROPERTY(BlueprintReadWrite)
	int32 X;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Y;

	// FOR NOW
	int32 GetDistance(const FGridPosition& Other) const
	{
		// Chebyshev distance
		return FMath::Max(FMath::Abs(Other.X - X), FMath::Abs(Other.Y - Y));
	}
	
	bool operator==(const FGridPosition& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
	
	bool operator<(const FGridPosition& Other) const
	{
		return (X < Other.X) ? true : (Y < Other.Y) ? true : false;
	}

	FGridPosition operator+(const FGridPosition& Other) const
	{
		return FGridPosition(X + Other.X, Y + Other.Y);
	}

	FGridPosition operator-(const FGridPosition& Other) const
	{
		return FGridPosition(X - Other.X, Y - Other.Y);
	}
};

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridPosition& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return HashCombineFast(GetTypeHash(Struct.X), GetTypeHash(Struct.Y));
}

USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FGridMovement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTileBase> GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridTileBase> ParentTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 0;
	
	
	bool operator==(const FGridMovement& Other) const
	{
		return GridTile == Other.GridTile;
	}
};

// Define a custom GetTypeHash function for hashing
inline uint32 GetTypeHash(const FGridMovement& Struct)
{
	// Use a combination of GetTypeHash for FString and int32
	return GetTypeHash(Struct.GridTile);
}

USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FGridPair
{
	GENERATED_BODY()
	
	FGridPair() {}
	FGridPair(AGridTileBase* InGridTile, AGridUnitBase* InGridUnit) : GridTile(InGridTile), GridUnit(InGridUnit) {}
	
	UPROPERTY(BlueprintReadOnly)
	AGridTileBase* GridTile = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AGridUnitBase* GridUnit = nullptr;
	
};

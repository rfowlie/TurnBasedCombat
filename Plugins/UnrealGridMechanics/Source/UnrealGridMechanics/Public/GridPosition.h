// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Math/IntVector.h"

USTRUCT(BlueprintType)
struct TURNBASED_CORE_API FGridPosition3D : public TIntVector3
{
	GENERATED_BODY()

	// FOR NOW
	int32 GetDistance(const FGridPosition& Other) const
	{
		// Chebyshev distance
		return FMath::Max(FMath::Abs(Other.X - X), FMath::Abs(Other.Y - Y));
	}
	
	bool operator<(const FGridPosition& Other) const
	{
		return (X < Other.X) ? true : (Y < Other.Y) ? true : false;
	}
};

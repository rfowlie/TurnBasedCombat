// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridTileUtility.generated.h"

class AGridTile;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UGridTileUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GridUnitUtility")
	static bool RegisterGridTile(AGridTile* InGridTile);
	
	UFUNCTION(BlueprintCallable, Category = "GridUnitUtility")
	static bool UnregisterGridTile(AGridTile* InGridTile);
};

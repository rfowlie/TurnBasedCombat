// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GigafirePawnHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UGigafirePawnHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GIGAFIRE_API IGigafirePawnHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCursorCanTick(const bool bActive);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFollowCursor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFollowTarget(AActor* InTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMoveToLocation(FVector Location);
};

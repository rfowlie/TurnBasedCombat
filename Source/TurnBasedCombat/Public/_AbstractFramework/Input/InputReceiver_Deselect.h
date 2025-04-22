// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputReceiver_Deselect.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInputReceiver_Deselect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDCOMBAT_API IInputReceiver_Deselect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PlayerController | Input")
	void OnReceiveInput_Deselect();
};

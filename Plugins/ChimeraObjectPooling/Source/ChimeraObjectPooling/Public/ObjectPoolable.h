// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolable.generated.h"


// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UObjectPoolable : public UInterface
{
	GENERATED_BODY()
};

class CHIMERAOBJECTPOOLING_API IObjectPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsActive();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Deactivate();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridInformationProvider.generated.h"

class UGridInterpreter;
class UGridMapBase;


// This class does not need to be modified.
UINTERFACE()
class UGridInformationProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDGRIDSYSTEMS_API IGridInformationProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual TSubclassOf<UGridMapBase> GetGridMapClass() = 0;
	// virtual TSubclassOf<UGridInterpreter> GetGridInterpreterClass() = 0;
};

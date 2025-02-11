// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"


/**
 * let us use data assets so that we can attach a variety of information tha might be costly (meshes, animations)
 * this way we can async load them when needed instead of loading an entire data table
 */
UCLASS()
class TURNBASED_CORE_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* StaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponTraits WeaponTraits = FWeaponTraits();
};

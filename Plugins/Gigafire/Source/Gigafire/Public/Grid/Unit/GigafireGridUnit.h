// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Unit/GridUnitBase.h"
#include "GigafireGridUnit.generated.h"

class UGigafireUnitAttributeSet;


UCLASS(Blueprintable, BlueprintType)
class GIGAFIRE_API AGigafireGridUnit : public AGridUnitBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGigafireGridUnit();

protected:
	virtual void BeginPlay() override;
	
public:
#pragma region GAS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGigafireUnitAttributeSet* GigafireUnitAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
#pragma endregion
	
};



USTRUCT(BlueprintType)
struct FGigafireGridUnitArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AGigafireGridUnit*> GridUnits;
	
};
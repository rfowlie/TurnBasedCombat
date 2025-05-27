// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WeightScoringMoves.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeightScoringMovesDelegate);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASEDCOMBAT_API UWeightScoringMoves : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UWeightScoringMoves* CreateWeightScoringMoves(const UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable)
	FWeightScoringMovesDelegate OnComplete;
	
	virtual void Activate() override;
};

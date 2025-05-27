// Fill out your copyright notice in the Description page of Project Settings.


#include "_Basic/AI/Utility/WeightScoringMoves.h"

UWeightScoringMoves* UWeightScoringMoves::CreateWeightScoringMoves(const UObject* WorldContextObject)
{
	UWeightScoringMoves* Obj = NewObject<UWeightScoringMoves>();

	return Obj;
}

void UWeightScoringMoves::Activate()
{
	Super::Activate();
}

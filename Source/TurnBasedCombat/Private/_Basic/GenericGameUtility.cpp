// Fill out your copyright notice in the Description page of Project Settings.


#include "_Basic/GenericGameUtility.h"


void UGenericGameUtility::MakeObjectScoreArray(TArray<FObjectScore>& OutObjectScores, const TMap<UObject*, int32>& ScoreMap)
{
	for (const auto& Elem : ScoreMap)
	{
		OutObjectScores.Add(FObjectScore(Elem.Key, Elem.Value));		
	}

	OutObjectScores.Sort();
}

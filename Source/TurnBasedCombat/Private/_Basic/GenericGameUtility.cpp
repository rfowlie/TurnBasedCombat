// Fill out your copyright notice in the Description page of Project Settings.


#include "_Basic/GenericGameUtility.h"


TArray<UObject*> UGenericGameUtility::GetSortedArrayOfMapKeys_Float(const TMap<UObject*, float>& Map)
{
	return GetSortedKeysByValue(Map);
}

void UGenericGameUtility::MakeObjectScoreArray(TArray<FObjectScore>& OutObjectScores, const TMap<UObject*, float>& ScoreMap)
{
	for (const auto& Elem : ScoreMap)
	{
		OutObjectScores.Add(FObjectScore(Elem.Key, Elem.Value));		
	}

	OutObjectScores.Sort();
}




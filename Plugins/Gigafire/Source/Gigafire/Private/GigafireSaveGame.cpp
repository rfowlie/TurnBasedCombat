// Fill out your copyright notice in the Description page of Project Settings.


#include "GigafireSaveGame.h"
#include "UnitTemplateDataAsset.h"
#include "UnitTemplateUtility.h"


bool UGigafireSaveGame::GetUnitData(FUnitLoadData& OutLoadData, FGameplayTag UnitTag)
{
	FUnitLoadData OutputData;
	for (auto SaveData : UnitSaveData)
	{
		if (SaveData.UnitTag == UnitTag)
		{
			UUnitTemplateUtility::MakeUnitLoadData(SaveData, OutLoadData);
			return true;
		}
	}

	return false;
}

TArray<FUnitLoadData> UGigafireSaveGame::GetAllUnitData()
{
	TArray<FUnitLoadData> OutputData;
	for (auto SaveData : UnitSaveData)
	{
		FUnitLoadData OutLoadData;
		UUnitTemplateUtility::MakeUnitLoadData(SaveData, OutLoadData);
		OutputData.Add(OutLoadData);
	}

	return OutputData;
}

TArray<FGameplayTag> UGigafireSaveGame::GetAllUnitTags()
{
	TArray<FGameplayTag> OutTags;
	for (auto SaveData : UnitSaveData)
	{
		OutTags.Add(SaveData.UnitTag);
	}
	return OutTags;
}

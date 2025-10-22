// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitTemplateUtility.h"
#include "UnitTemplateDataAsset.h"


void UUnitTemplateUtility::MakeUnitSaveData(const FUnitLoadData& InLoadData, FUnitSaveData& OutSaveData)
{
	for (const auto Pair : InLoadData.Templates)
	{
		FGameplayTagValuePair TagValuePair;
		TagValuePair.Tag = Pair.Key;
		TagValuePair.Value = Pair.Value;
		OutSaveData.Templates.AddUnique(TagValuePair);
	}

	for (const auto Pair : InLoadData.PermanentStats)
	{
		FGameplayTagValuePair TagValuePair;
		TagValuePair.Tag = Pair.Key;
		TagValuePair.Value = Pair.Value;
		OutSaveData.PermanentStats.AddUnique(TagValuePair);
	}

	for (const auto Pair : InLoadData.PermanentTraits)
	{
		FGameplayTagValuePair TagValuePair;
		TagValuePair.Tag = Pair.Key;
		TagValuePair.Value = Pair.Value;
		OutSaveData.PermanentTraits.AddUnique(TagValuePair);
	}

	for (const auto Pair : InLoadData.PermanentSkills)
	{
		FGameplayTagValuePair TagValuePair;
		TagValuePair.Tag = Pair.Key;
		TagValuePair.Value = Pair.Value;
		OutSaveData.PermanentSkills.AddUnique(TagValuePair);
	}

	OutSaveData.UnitTag = InLoadData.UnitTag;
	OutSaveData.EquippedData = InLoadData.EquippedData;
}

void UUnitTemplateUtility::MakeUnitLoadData(const FUnitSaveData& InSaveData, FUnitLoadData& OutLoadData)
{
	for (const auto Pair : InSaveData.Templates)
	{
		OutLoadData.Templates.Add(Pair.Tag, Pair.Value);
	}
	for (const auto Pair : InSaveData.PermanentStats)
	{
		OutLoadData.PermanentStats.Add(Pair.Tag, Pair.Value);
	}
	for (const auto Pair : InSaveData.PermanentTraits)
	{
		OutLoadData.PermanentTraits.Add(Pair.Tag, Pair.Value);
	}
	for (const auto Pair : InSaveData.PermanentSkills)
	{
		OutLoadData.PermanentSkills.Add(Pair.Tag, Pair.Value);
	}

	OutLoadData.UnitTag = InSaveData.UnitTag;
	OutLoadData.EquippedData = InSaveData.EquippedData;
}

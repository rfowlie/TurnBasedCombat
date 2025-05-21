// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericGameUtility.generated.h"


USTRUCT(BlueprintType)
struct FObjectScore
{
	GENERATED_BODY()

	FObjectScore() {}
	FObjectScore(UObject* InObject, const int32 InScore) : Object(InObject), Score(InScore) {}
	FObjectScore(const FObjectScore& Other) : Object(Other.Object), Score(Other.Score) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* Object = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Score = 0;

	bool operator==(const FObjectScore& Other) const
	{
		return Score == Other.Score;
	}
	
	auto operator<(const FObjectScore& Other) const
	{
		return Score < Other.Score;
	}
	
};

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UGenericGameUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void MakeObjectScoreArray(TArray<FObjectScore>& OutObjectScores, const TMap<UObject*, int32>& ScoreMap);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/CombatEvaluator.h"
#include "Components/ActorComponent.h"
#include "GridUnitBehaviourComponent.generated.h"


UENUM()
enum class ECombatBehaviour
{
	Neutral,
	Aggressive,
	Conservative
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASED_CORE_API UGridUnitBehaviourComponent : public UActorComponent, public ICombatEvaluator
{
	GENERATED_BODY()

public:
	UGridUnitBehaviourComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECombatBehaviour CombatBehaviour = ECombatBehaviour::Neutral;

	UFUNCTION(BlueprintCallable)
	virtual void GetCombatEvaluation(FCombatEvaluation& OutCombatEvaluation, const FCombatPrediction& InCombatPrediction) override;
	
private:

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetKeyOptions="GetCombatBehaviourOptions"))
	// TArray<FName> CombatBehaviour;
	
	// UFUNCTION(BlueprintCallable)
	// static TArray<FName> GetCombatBehaviourOptions()
	// {
	// 	return TArray<FName> {
	// 		FName("Neutral"),
	// 		FName("Aggressive"),
	// 		FName("Conservative"),
	// 	};
	// }

	void GetCombatEvaluation_Neutral(FCombatEvaluation& CombatEvaluation);
	void GetCombatEvaluation_Aggressive(FCombatEvaluation& CombatEvaluation);
	void GetCombatEvaluation_Conservative(FCombatEvaluation& CombatEvaluation);
};

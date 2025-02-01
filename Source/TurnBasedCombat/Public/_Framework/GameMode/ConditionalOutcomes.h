#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "ConditionalOutcomes.generated.h"

// Define an outcome type enum
UENUM(BlueprintType)
enum class EConditionOutcomeType : uint8
{
	None,
	Fail,
	PartialFail,
	PartialSuccess,
	Success
};

// Struct containing outcome mappings
USTRUCT(BlueprintType)
struct FConditionalOutcomesMap
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, EConditionOutcomeType> Map;
};

// Declare a delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConditionalOutcomesDelegate, FConditionalOutcomesMap, ConditionalOutcomes);

// Unreal interface metadata (must be empty)
UINTERFACE(BlueprintType)
class UConditionalOutcomes : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface definition
 */
class TURNBASEDCOMBAT_API IConditionalOutcomes
{
	GENERATED_BODY()

public:
	// Function to return a reference to the delegate
	// UFUNCTION(BlueprintCallable, Category = "Conditional Outcomes")
	virtual FConditionalOutcomesDelegate& GetConditionalOutcomesDelegate() = 0;
};

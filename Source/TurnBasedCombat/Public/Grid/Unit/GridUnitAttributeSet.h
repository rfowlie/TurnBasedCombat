// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Stats/StatsDataAsset.h"

#include "GridUnitAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


struct FUnitStatsSnapshot;
class UStatsDataAsset;
/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UGridUnitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGridUnitAttributeSet();

	// overrides
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// attributes
	UPROPERTY(ReplicatedUsing="OnRep_Health", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGridUnitAttributeSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_HealthMax", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UGridUnitAttributeSet, HealthMax);
	UFUNCTION()
	void OnRep_HealthMax(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Strength", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UGridUnitAttributeSet, Strength);
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Defence", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UGridUnitAttributeSet, Defence);
	UFUNCTION()
	void OnRep_Defence(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Movement", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Movement;
	ATTRIBUTE_ACCESSORS(UGridUnitAttributeSet, Movement);
	UFUNCTION()
	void OnRep_Movement(const FGameplayAttributeData& PreviousValue) const;

	// custom
	UPROPERTY(BlueprintReadOnly)
	FUnitStatsSnapshot StatsSnapshot;
	
	UFUNCTION(BlueprintCallable)
	void InitializeAttributesFromStatsDataAsset(UStatsDataAsset* StatsDataAsset, const int32 Level);

	UPROPERTY()
	bool IsDefeated;
	DECLARE_EVENT(UGridUnitAttributeSet, FGridUnitAttributeEvent)
	FGridUnitAttributeEvent OnHealthZero;
};

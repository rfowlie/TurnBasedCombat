// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "UGigafireUnitAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


struct FGigafireUnitAttributes;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGigafireUnitAttributeDelegate);

/**
 * 
 */
UCLASS()
class GIGAFIRE_API UGigafireUnitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGigafireUnitAttributeSet();

	// overrides
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// attributes
	UPROPERTY(ReplicatedUsing="OnRep_Health", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_HealthMax", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, HealthMax);
	UFUNCTION()
	void OnRep_HealthMax(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Strength", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Strength);
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Skill", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Skill;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Skill);
	UFUNCTION()
	void OnRep_Skill(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Speed", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Speed);
	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Luck", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Luck);
	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Defence", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Defence);
	UFUNCTION()
	void OnRep_Defence(const FGameplayAttributeData& PreviousValue) const;
	
	UPROPERTY(ReplicatedUsing="OnRep_Resistance", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Resistance);
	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& PreviousValue) const;
	
	UPROPERTY(ReplicatedUsing="OnRep_Movement", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Movement;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Movement);
	UFUNCTION()
	void OnRep_Movement(const FGameplayAttributeData& PreviousValue) const;

	UPROPERTY(ReplicatedUsing="OnRep_Constitution", BlueprintReadOnly, Category="Attributes | Core")
	FGameplayAttributeData Constitution;
	ATTRIBUTE_ACCESSORS(UGigafireUnitAttributeSet, Constitution);
	UFUNCTION()
	void OnRep_Constitution(const FGameplayAttributeData& PreviousValue) const;

	// helper
	UFUNCTION(BlueprintCallable)
	void InitializeAttributeSet(const FGigafireUnitAttributes& InAttributes);

protected:
	UPROPERTY()
	bool IsDefeated = false;
	
public:
	UPROPERTY(BlueprintAssignable)
	FGigafireUnitAttributeDelegate OnHealthZero;

	UFUNCTION(BlueprintCallable)
	bool IsUnitDefeated() const;
	
};

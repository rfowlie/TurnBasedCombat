// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LerpMovementHelper.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLerpFinished);

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API ULerpMovementHelper : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTickableThing, STATGROUP_Tickables); }
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	UPROPERTY()
	bool IsActive = false;
	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Conditional;
	}

	// Start lerping
	UFUNCTION(BlueprintCallable, Category = "LerpMovementHelper")
	void StartLerp(AActor* InActor, FVector InStartLocation, FVector InEndLocation, float InDuration);

	// Event fired when lerp is finished
	UPROPERTY(BlueprintAssignable, Category = "LerpMovementHelper")
	FOnLerpFinished OnLerpFinished;

protected:
	UPROPERTY()
	AActor* TargetActor;

	FVector Start;
	FVector End;
	float LerpDuration;
	float ElapsedTime;

	FTimerHandle TimerHandle;
	
private:
	// The last frame number we were ticked.
	// We don't want to tick multiple times per frame 
	uint32 LastFrameNumberWeTicked = INDEX_NONE;
};

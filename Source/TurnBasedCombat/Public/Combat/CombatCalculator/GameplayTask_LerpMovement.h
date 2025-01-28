// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "GameplayTask_LerpMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLerpMovementDelegate);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API UGameplayTask_LerpMovement : public UGameplayTask
{
	GENERATED_BODY()

public:
	// Factory method to create and start the task
	UFUNCTION(BlueprintCallable, Category = "Gameplay Task", meta = (DisplayName = "Lerp Actor", BlueprintInternalUseOnly = "true"))
	static UGameplayTask_LerpMovement* LerpActor(
		UObject* WorldContextObject,
		AActor* ActorToMove,
		const FVector& StartLocation,
		const FVector& TargetLocation,
		float LerpDuration,
		const bool bRotateToFace = true,
		float RotationDuration = 0.2f);

	// Activate the task
	virtual void Activate() override;

	// Tick the task
	virtual void TickTask(float DeltaTime) override;

protected:
	// Actor to move
	UPROPERTY()
	AActor* Actor = nullptr;

	// Start and target locations
	FVector StartPos = FVector();
	FVector TargetPos = FVector();

	// Time-related variables
	float MovementDuration = 0.f;
	float ElapsedTime = 0.f;

	// Rotation variables
	bool bRotateToFace = false;
	FRotator StartRotation = FRotator();
	FRotator TargetRotation = FRotator();
	float RotationInterpolationSpeed = 0.f;

	// Notify when the task is complete
	UPROPERTY(BlueprintAssignable)
	FLerpMovementDelegate OnLerpCompleted;

private:
	// Called when the lerp is complete
	void FinishLerp();
};

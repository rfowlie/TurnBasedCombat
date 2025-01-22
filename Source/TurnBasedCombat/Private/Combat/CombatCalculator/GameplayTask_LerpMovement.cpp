// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/GameplayTask_LerpMovement.h"

#include "Kismet/KismetMathLibrary.h"


UGameplayTask_LerpMovement* UGameplayTask_LerpMovement::LerpActor(
	UObject* WorldContextObject, AActor* ActorToMove, const FVector& StartLocation,
	const FVector& TargetLocation, float LerpDuration, const bool bRotateToFace, const float RotationDuration)
{
	if (!ActorToMove)
	{
		return nullptr;
	}

	UGameplayTask_LerpMovement* MyTask = NewObject<UGameplayTask_LerpMovement>();
	MyTask->Actor = ActorToMove;
	MyTask->StartPos = StartLocation;
	MyTask->TargetPos = TargetLocation;
	MyTask->MovementDuration = LerpDuration;
	MyTask->ElapsedTime = 0.0f;
	MyTask->bRotateToFace = bRotateToFace;
	MyTask->RotationInterpolationSpeed = 1.0f/RotationDuration;
	MyTask->StartRotation = ActorToMove->GetActorRotation();
	MyTask->TargetRotation = UKismetMathLibrary::FindLookAtRotation(ActorToMove->GetActorLocation(), TargetLocation);
	MyTask->TargetRotation.Pitch = 0.0f; // Lock pitch
	MyTask->TargetRotation.Roll = 0.0f; // Lock roll

	MyTask->bTickingTask = true;
	MyTask->ReadyForActivation();
	return MyTask;
}

void UGameplayTask_LerpMovement::Activate()
{
	Super::Activate();
	if (!Actor)
	{
		EndTask();
		// return;
	}
	
	// Super::Activate();
}

void UGameplayTask_LerpMovement::TickTask(float DeltaTime)
{
	if (!Actor || MovementDuration <= 0.0f)
	{
		FinishLerp();
		return;
	}

	ElapsedTime += DeltaTime;

	float Alpha = FMath::Clamp(ElapsedTime / MovementDuration, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(StartPos, TargetPos, Alpha);
	Actor->SetActorLocation(NewLocation);
	

	if (bRotateToFace)
	{
		FRotator NewRotation = FMath::RInterpTo(
			StartRotation, TargetRotation, DeltaTime, RotationInterpolationSpeed); // Adjust interp speed
		Actor->SetActorRotation(NewRotation);
	}

	if (Alpha >= 1.0f)
	{
		FinishLerp();
	}
}

void UGameplayTask_LerpMovement::FinishLerp()
{
	if (OnLerpCompleted.IsBound())
	{
		OnLerpCompleted.Broadcast();
	}

	EndTask();
}
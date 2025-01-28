// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatCalculator/LerpMovementHelper.h"


void ULerpMovementHelper::Tick(float DeltaTime)
{
	// prevent double ticking...
	if (LastFrameNumberWeTicked == GFrameCounter) { return; }
	LastFrameNumberWeTicked = GFrameCounter;

	// do stuff...
	if (!TargetActor)
	{
		OnLerpFinished.Broadcast();
		return;
	}
	
	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / LerpDuration, 0.0f, 1.0f);
	TargetActor->SetActorLocation(FMath::Lerp(Start, End, Alpha));

	if (1.0f < Alpha)
	{
		// stop ticking
		IsActive = false;
		OnLerpFinished.Broadcast();
	}
}

bool ULerpMovementHelper::IsTickable() const
{
	return IsActive;
}

void ULerpMovementHelper::StartLerp(AActor* InActor, FVector InStartLocation, FVector InEndLocation, float InDuration)
{
	if (IsActive) { return; }
	if (!InActor || InDuration <= 0.0f)
	{
		IsActive = false;
		OnLerpFinished.Broadcast();
		return;
	}

	TargetActor = InActor;
	Start = InStartLocation;
	End = InEndLocation;
	LerpDuration = InDuration;
	ElapsedTime = 0.0f;		
	IsActive = true;
}
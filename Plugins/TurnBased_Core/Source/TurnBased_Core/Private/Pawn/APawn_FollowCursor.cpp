// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/APawn_FollowCursor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


APawn_FollowCursor::APawn_FollowCursor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the Spring Arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1200.0f;  // Adjust for desired zoom height
	SpringArmComponent->bDoCollisionTest = false;  // No need for collision in grid-based games

	// Create Camera and Attach to Spring Arm
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	// default state
	SetFollowCursor();
}

void APawn_FollowCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bCursorCanTick && FollowDelegate.IsBound()) { FollowDelegate.Execute(DeltaTime); }	
}

void APawn_FollowCursor::SetMapBounds(FVector2D MinBounds, FVector2D MaxBounds)
{
	MapMinBounds = MinBounds;
	MapMaxBounds = MaxBounds;
}

void APawn_FollowCursor::SetFollowCursor()
{
	FollowDelegate.Unbind();
	FollowDelegate.BindUObject(this, &ThisClass::HandleFollowCursor);
	SetCursorCanTick(true);
}

void APawn_FollowCursor::HandleFollowCursor(float DeltaTime)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);
	int32 ScreenX, ScreenY;
	PC->GetViewportSize(ScreenX, ScreenY);
	FVector2D PositionPercentage = FVector2D(MouseX/ScreenX, MouseY/ScreenY);

	FVector MoveDirection = FVector::ZeroVector;
	// flip directions as in screenspace X is left right and Y is up down
	MoveDirection.Y = PositionPercentage.X < EdgeThresholdPercentX ? -1.f :
		PositionPercentage.X > 1.f - EdgeThresholdPercentX ? 1.f : 0;
	MoveDirection.X = PositionPercentage.Y < EdgeThresholdPercentY ? 1.f :
		PositionPercentage.Y > 1.f - EdgeThresholdPercentY ? -1.f : 0;
	
	FVector NewLocation = GetActorLocation() + (MoveDirection * CursorFollowSpeed * DeltaTime);

	// Clamp movement within map boundaries
	NewLocation.X = FMath::Clamp(NewLocation.X, MapMinBounds.X, MapMaxBounds.X);
	NewLocation.Y = FMath::Clamp(NewLocation.Y, MapMinBounds.Y, MapMaxBounds.Y);

	SetActorLocation(NewLocation);
}

void APawn_FollowCursor::SetFollowTarget(AActor* InTarget)
{
	if (!IsValid(InTarget))
	{
		if (OnFollowTargetComplete.IsBound()) { OnFollowTargetComplete.Broadcast(); }
		return;
	}

	FollowTarget = InTarget;
	FollowDelegate.Unbind();
	FollowDelegate.BindUObject(this, &ThisClass::HandleFollowTarget);
	SetCursorCanTick(true);
}

void APawn_FollowCursor::HandleFollowTarget(float DeltaTime)
{
	if (IsValid(FollowTarget))
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), FollowTarget->GetActorLocation(), DeltaTime, TargetFollowSpeed);
		NewLocation.Z = GetActorLocation().Z;
		SetActorLocation(NewLocation);

		FVector2D Self = FVector2D(GetActorLocation());
		FVector2D Target = FVector2D(FollowTarget->GetActorLocation());
		if (FMath::Abs((Target - Self).Size()) < FollowThreshold)
		{
			if (OnFollowTargetComplete.IsBound()) { OnFollowTargetComplete.Broadcast(); }
		}
	}
	else
	{
		FollowTarget = nullptr;
		FollowDelegate.BindUObject(this, &ThisClass::HandleFollowCursor);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/FollowCamera_Cursor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AFollowCamera_Cursor::AFollowCamera_Cursor()
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
}

void AFollowCamera_Cursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMouseMovement(DeltaTime);
}

void AFollowCamera_Cursor::SetMapBounds(FVector2D MinBounds, FVector2D MaxBounds)
{
	MapMinBounds = MinBounds;
	MapMaxBounds = MaxBounds;
}

void AFollowCamera_Cursor::BeginPlay()
{
	Super::BeginPlay();
}

void AFollowCamera_Cursor::HandleMouseMovement(float DeltaTime)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);

	int32 ScreenX, ScreenY;
	PC->GetViewportSize(ScreenX, ScreenY);

	FVector MoveDirection = FVector::ZeroVector;

	if (MouseX <= EdgeThreshold) MoveDirection.Y = -1;  // Move Left
	if (MouseX >= ScreenX - EdgeThreshold) MoveDirection.Y = 1;  // Move Right
	if (MouseY <= EdgeThreshold) MoveDirection.X = -1;  // Move Up
	if (MouseY >= ScreenY - EdgeThreshold) MoveDirection.X = 1;  // Move Down

	FVector NewLocation = GetActorLocation() + (MoveDirection * MouseScrollSpeed * DeltaTime);

	// Clamp movement within map boundaries
	NewLocation.X = FMath::Clamp(NewLocation.X, MapMinBounds.X, MapMaxBounds.X);
	NewLocation.Y = FMath::Clamp(NewLocation.Y, MapMinBounds.Y, MapMaxBounds.Y);

	SetActorLocation(NewLocation);
}

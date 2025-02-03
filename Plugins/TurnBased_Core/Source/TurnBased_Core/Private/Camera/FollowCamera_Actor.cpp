// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/FollowCamera_Actor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AFollowCamera_Actor::AFollowCamera_Actor()
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

void AFollowCamera_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFollowCamera_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor)
	{
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, FollowSpeed);
		SetActorLocation(NewLocation);
	}
}

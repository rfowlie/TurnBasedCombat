﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APawn_FollowCursor.generated.h"


UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API APawn_FollowCursor : public APawn
{
	GENERATED_BODY()

public:
	APawn_FollowCursor();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMapBounds(FVector2D MinBounds, FVector2D MaxBounds);

	UFUNCTION(BlueprintCallable)
	void SetFollowTarget(AActor* InTarget);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere, Category="_Settings")
	float FollowSpeed = 5.0f;  // Smooth camera follow speed

	UPROPERTY(EditAnywhere, Category="_Settings")
	float CursorFollowSpeed = 2000.0f;  // Speed of camera movement with mouse

	// distance from each screen edge we want to trigger at
	UPROPERTY(EditAnywhere, Category="_Settings", meta=(ClampMin=0.f, ClampMax=0.5f, UIMin=0.f, UIMax=0.5f))
	float EdgeThresholdPercentX = 0.15f;
	UPROPERTY(EditAnywhere, Category="_Settings", meta=(ClampMin=0.f, ClampMax=0.5f, UIMin=0.f, UIMax=0.5f))
	float EdgeThresholdPercentY = 0.1f;
	
	UPROPERTY(EditAnywhere, Category="_Settings")
	FVector2D MapMinBounds = FVector2D(-5000, -5000);
	UPROPERTY(EditAnywhere, Category="_Settings")
	FVector2D MapMaxBounds = FVector2D(5000, 5000);

	UFUNCTION()
	void HandleFollowCursor(float DeltaTime);
	
	UPROPERTY()
	AActor* FollowTarget;

	UPROPERTY(EditAnywhere, Category="_Settings")
	float TargetFollowSpeed = 5.0f;
	
	UFUNCTION()
	void HandleFollowTarget(float DeltaTime);
	
	enum EFollowMode
	{
		Cursor,
		Target
	};	
	
	EFollowMode FollowMode = EFollowMode::Cursor;

	DECLARE_DELEGATE_OneParam(FFollowDelegate, float);
	FFollowDelegate FollowDelegate;
};

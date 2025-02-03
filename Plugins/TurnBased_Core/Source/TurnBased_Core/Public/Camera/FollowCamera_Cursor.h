// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowCamera_Cursor.generated.h"


UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API AFollowCamera_Cursor : public AActor
{
	GENERATED_BODY()

public:
	AFollowCamera_Cursor();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMapBounds(FVector2D MinBounds, FVector2D MaxBounds);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere)
	float FollowSpeed = 5.0f;  // Smooth camera follow speed

	UPROPERTY(EditAnywhere)
	float MouseScrollSpeed = 2000.0f;  // Speed of camera movement with mouse

	UPROPERTY(EditAnywhere)
	float EdgeThreshold = 50.0f;  // Distance from edge before moving

	FVector2D MapMinBounds;
	FVector2D MapMaxBounds;

	void HandleMouseMovement(float DeltaTime);
};

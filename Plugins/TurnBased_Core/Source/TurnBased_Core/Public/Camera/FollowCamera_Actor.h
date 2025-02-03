// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowCamera_Actor.generated.h"


UCLASS(Blueprintable, BlueprintType)
class TURNBASED_CORE_API AFollowCamera_Actor : public AActor
{
	GENERATED_BODY()

public:
	AFollowCamera_Actor();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* InActor) { TargetActor = InActor; }
	
	UFUNCTION(BlueprintCallable)
	void SetFollowSpeed(float InFollowSpeed) { FollowSpeed = InFollowSpeed; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	float FollowSpeed = 5.0f;
	
	UPROPERTY()
	AActor* TargetActor;
	
};

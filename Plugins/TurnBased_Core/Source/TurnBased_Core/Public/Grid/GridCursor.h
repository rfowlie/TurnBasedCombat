// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCursor.generated.h"


UCLASS()
class TURNBASED_CORE_API AGridCursor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridCursor();

	UFUNCTION()
	void ShowTileCursor(bool bShow);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateCursor();
	
	UPROPERTY(BlueprintReadWrite, Category="TurnBased | Cursor")
	bool TileCursorVisible = true;

	UFUNCTION()
	void UpdateCursor(AGridTile* GridTile);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Scale = FVector::OneVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector HeightAboveTile = FVector(0.f, 0.f, 5.f);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

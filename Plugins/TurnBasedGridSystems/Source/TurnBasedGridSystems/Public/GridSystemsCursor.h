// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSystemsCursor.generated.h"


class AGridTileBase;

UCLASS()
class TURNBASEDGRIDSYSTEMS_API AGridSystemsCursor : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	AGridSystemsCursor();

	UFUNCTION()
	void ShowTileCursor(bool bShow);

protected:
	void CreateCursor();
	
	UPROPERTY(BlueprintReadWrite, Category="TurnBased | Cursor")
	bool TileCursorVisible = true;

	UFUNCTION()
	void UpdateCursor(AGridTileBase* InGridTile);
	
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

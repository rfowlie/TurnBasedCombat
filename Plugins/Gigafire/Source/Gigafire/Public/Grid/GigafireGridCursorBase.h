// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GigafireCursorHandler.h"
#include "GigafireGridCursorBase.generated.h"

class AGridTileBase;


UCLASS()
class GIGAFIRE_API AGigafireGridCursorBase : public AActor, public IGigafireCursorHandler
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGigafireGridCursorBase();

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gigafire | Cursor")
	TObjectPtr<UStaticMesh> CursorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gigafire | Cursor")
	FVector TileCursorScale = FVector::OneVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gigafire | Cursor")
	FVector Cursor_ExtraHeight = FVector(0.f, 0.f, 5.f);

	// stop updating and hide actor
	UFUNCTION(BlueprintCallable, Category = "Gigafire | Cursor")
	void EnableCursor(bool bValue);

	// stop updating but show actor
	UFUNCTION(BlueprintCallable, Category = "Gigafire | Cursor")
	void PauseCursor(bool bValue);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Gigafire | Cursor")
	bool IsEnabled = true;

	UFUNCTION(BlueprintCallable, Category = "Gigafire | Cursor")
	void CreateCursor();

	UFUNCTION(BlueprintCallable, Category = "Gigafire | Cursor")
	void UpdateCursor(AGridTileBase* GridTile);
};

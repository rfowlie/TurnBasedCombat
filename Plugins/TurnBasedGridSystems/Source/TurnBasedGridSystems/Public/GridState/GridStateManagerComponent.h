// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridStateManagerComponent.generated.h"

class UGridStateChangeHandler;
class IGridStateChangeHandler;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract, Blueprintable, BlueprintType)
class TURNBASEDGRIDSYSTEMS_API UGridStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UGridStateManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MustImplement = "GridStateChangeHandler") )
	TSubclassOf<UObject> DefaultGridStateHandler;

	UFUNCTION(BlueprintCallable)
	void SetGridStateHandler(UObject* InGridStateChangeHandler);

protected:
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IGridStateChangeHandler> GridStateChangeHandler;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/LevelManagerActor.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ALevelManagerActor::ALevelManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ALevelManagerActor* ALevelManagerActor::Get(const UObject* WorldContext)
{
	ALevelManagerActor* LevelManagerActor = nullptr;
	if (WorldContext)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(
			WorldContext->GetWorld(), ALevelManagerActor::StaticClass(), OutActors);
		if (!OutActors.IsEmpty())
		{
			LevelManagerActor = Cast<ALevelManagerActor>(OutActors[0]);
		}
	}

	return LevelManagerActor;
}

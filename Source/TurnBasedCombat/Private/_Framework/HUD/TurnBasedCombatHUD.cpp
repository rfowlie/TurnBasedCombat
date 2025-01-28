// Fill out your copyright notice in the Description page of Project Settings.


#include "_Framework/HUD/TurnBasedCombatHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "_Framework/PlayerController/UI/CombatDisplayWidget.h"

void ATurnBasedCombatHUD::BeginPlay()
{
	Super::BeginPlay();

	// so you don't have to click in to screen in editor...
	// UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	
	// if (CombatDisplayWidgetClass)
	// {
	// 	UUserWidget* Temp = CreateWidget(GetOwningPlayerController(), CombatDisplayWidgetClass, TEXT("CombatDisplayWidget"));
	// 	CombatDisplayWidget = Cast<UCombatDisplayWidget>(Temp);
	// }
	//
	// if (!CombatDisplayWidget)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Player Controller: combat display widget failed to create!!"))
	// }
}
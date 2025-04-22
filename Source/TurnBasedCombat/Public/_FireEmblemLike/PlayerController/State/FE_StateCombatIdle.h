// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_AbstractFramework/Input/InputReceiver_Select.h"
#include "_AbstractFramework/PlayerController/State/StackStateBase.h"
#include "FE_StateCombatIdle.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UFE_StateCombatIdle : public UStackStateBase, public IInputReceiver_Select
{
	GENERATED_BODY()

public:
	virtual void OnEnter(APlayerController* InPlayerController) override;

	virtual void OnReceiveInput_Select_Implementation() override;
};

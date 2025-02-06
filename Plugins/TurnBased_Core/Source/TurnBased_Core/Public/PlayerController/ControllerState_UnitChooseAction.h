// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBased_Core_Tags.h"
#include "PlayerController/ControllerState_Abstract.h"
#include "ControllerState_UnitChooseAction.generated.h"


class AGridUnit;
class UWidget_ActionOptions;

/**
 * 
 */
UCLASS()
class TURNBASED_CORE_API UControllerState_UnitChooseAction : public UControllerState_Abstract
{
	GENERATED_BODY()

	UControllerState_UnitChooseAction();

public:
	static UControllerState_UnitChooseAction* Create(AGridUnit* InActiveUnit);

	virtual void OnEnter(APlayerController* InPlayerController, const int32 InInputMappingContextPriority) override;
	
	virtual void OnExit() override;

protected:
	UPROPERTY()
	AGridUnit* ActiveUnit = nullptr;
	
	UPROPERTY()
	UWidget_ActionOptions* Widget_ActionOptions = nullptr;

	UFUNCTION()
	void OnActionSelected(FGameplayTag ActionTag);

private:
	TArray<FGameplayTag> GetActionTags() {
		TArray<FGameplayTag> OutTags;
		// OutTags.Add(TAG_TBCore_Action_Move);
		OutTags.Add(TAG_TBCore_Action_Attack);
		OutTags.Add(TAG_TBCore_Action_Skill);
		OutTags.Add(TAG_TBCore_Action_Item);
		return OutTags;
	};

};

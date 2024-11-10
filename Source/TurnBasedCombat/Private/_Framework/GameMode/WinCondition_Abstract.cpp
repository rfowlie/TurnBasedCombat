// Fill out your copyright notice in the Description page of Project Settings.


#include "_Framework/GameMode/WinCondition_Abstract.h"

void UWinCondition_Abstract::CallWinCondition(EWinConditionType WinCondition)
{
	if (OnWinCondition.IsBound()) { OnWinCondition.Execute(WinCondition); }
}

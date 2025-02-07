// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD_TurnBased.h"

#include "Combat/CombatCalculator_Basic.h"

AHUD_TurnBased::AHUD_TurnBased()
{
	CombatCalculator = CreateDefaultSubobject<UCombatCalculator_Basic>(TEXT("CombatCalculator"));
}

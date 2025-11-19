// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Unit/GigafireGridUnit.h"
#include "Grid/Unit/UGigafireUnitAttributeSet.h"


AGigafireGridUnit::AGigafireGridUnit()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	GigafireUnitAttributeSet = CreateDefaultSubobject<UGigafireUnitAttributeSet>(TEXT("AttributeSet_GridUnit"));
	
}

void AGigafireGridUnit::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->AddAttributeSetSubobject(GigafireUnitAttributeSet);
	
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"


// grid - state
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Grid_State_Idle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Grid_Tile_CanMove);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Grid_Tile_CanAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Grid_Tile_Combat);

// grid - event
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Grid_Death);

// tile

// unit

// actions
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Action_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Action_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Action_Skill);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Action_Item);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_TBCore_Action_Wait);

// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBased_Core_Tags.h"


// grid - state
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Grid_State_Idle, "TBCore.Grid.Tile.State.Idle");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Grid_Tile_CanMove, "TBCore.Grid.Tile.State.CanMove");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Grid_Tile_CanAttack, "TBCore.Grid.Tile.State.CanAttack");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Grid_Tile_Combat, "TBCore.Grid.Tile.State.Combat");


// grid - ability
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Grid_Move, "Event.Grid.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Grid_Attack, "Event.Grid.Attack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Grid_Damage, "Event.Grid.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Grid_Death, "Event.Grid.Death");

// tile

// unit

// actions
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Action_Move, "TBCore.Action.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Action_Attack, "TBCore.Action.Attack");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Action_Skill, "TBCore.Action.Skill");
UE_DEFINE_GAMEPLAY_TAG(TAG_TBCore_Action_Item, "TBCore.Action.Item");
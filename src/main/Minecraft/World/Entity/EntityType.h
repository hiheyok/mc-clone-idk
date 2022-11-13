#pragma once

#include "Entity.h"
#include "EntityType/Player.h"
#include "EntityType/FallingBlock.h"
#include "../../Core/Registry.h"

const ID PLAYER = EntityRegistry.Register(new Player(), "player");
const ID FALLING_BLOCK = EntityRegistry.Register(new FallingBlock, "falling_block");
#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "Blocks/AirBlock.h"
#include "Blocks/GrassBlock.h"
#include "Blocks/DirtBlock.h"
#include "Blocks/GravityBlock.h"
#include "Blocks/FluidBlock.h"
#include "Blocks/LeafBlock.h"
#include "BlockStatus.h"
#include "../../../Core/Registry.h"

const BlockID AIR = BlockRegistry.Register(new AirBlock(), "air");
const BlockID DIRT = BlockRegistry.Register(new DirtBlock(), "dirt");
const BlockID GRASS_BLOCK = BlockRegistry.Register(new GrassBlock(), "grass_block");
const BlockID COBBLESTONE = BlockRegistry.Register(new Block(), "cobblestone");
const BlockID STONE = BlockRegistry.Register(new Block(), "stone");
const BlockID SAND = BlockRegistry.Register(new GravityBlock(), "sand");
const BlockID WATER = BlockRegistry.Register(new FluidBlock(), "water");
const BlockID OAK_LOG = BlockRegistry.Register(new Block(), "oak_log");
const BlockID OAK_LEAF = BlockRegistry.Register(new LeafBlock(), "oak_leaf");


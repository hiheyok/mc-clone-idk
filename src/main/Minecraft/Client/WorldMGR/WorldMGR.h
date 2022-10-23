#pragma once
#include "../../World/World.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <unordered_map>

class WorldManager {
public:
	void JoinWorld();
	void CreateWorld();

	World* world;
};
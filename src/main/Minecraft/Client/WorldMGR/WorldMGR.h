#pragma once
#include "../../World/World.h"

#include <unordered_map>

class WorldManager {
public:
	void JoinWorld();
	void CreateWorld();

	World* world;
};
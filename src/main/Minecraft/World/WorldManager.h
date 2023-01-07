#pragma once
#include "../Core/Registry.h"
#include "SingleplayerWorld.h"

class WorldManager {
public:
	SingleplayerWorld* CreateNewWorld() {
		SingleplayerWorld* NewWorld = new SingleplayerWorld();
	}
};
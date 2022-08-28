#pragma once
#include "WorldContainer.h"

class World : WorldContainer {
public:

	const unsigned int TPS = 20;

	World() {
	}
	void Initialize();

	void StartGenThreads(int amount);

	void WorldLoop();

	bool stop = false;

private:
	const int SpawnChunksSize = 10;
	std::thread WorldMainThread;
};
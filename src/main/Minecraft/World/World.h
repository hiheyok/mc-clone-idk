#pragma once
#include "WorldContainer.h"

class World : WorldContainer {
public:

	const unsigned int TPS = 20;

	World() {
		StartGenThreads(4);
	}
	void Initialize();

	void StartGenThreads(int amount);

	void WorldLoop();

	bool stop = false;

private:
	const int SpawnChunksSize = 40;
	std::thread WorldMainThread;
};
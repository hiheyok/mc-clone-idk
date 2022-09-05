#pragma once
#include "WorldContainer.h"

class World : public WorldContainer {
public:

	const unsigned int TPS = 20;

	void Initialize();

	void StartGenThreads(int amount);

	void WorldLoop();

	bool stop = false;

private:

	void SendChunkDataToClient();

	const int SpawnChunksSize = 10;
	std::thread WorldMainThread;
};
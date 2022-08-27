#include "World.h"

void World::StartGenThreads(int amount) {
	for (int i = 0; i < amount; i++) {
		AddWorldGenWorker();
	}
}

void World::Initialize() {
	//Load 10x10x10 Chunks
	for (int x = -SpawnChunksSize; x < SpawnChunksSize; x++) {
		for (int y = -SpawnChunksSize; y < SpawnChunksSize; y++) {
			for (int z = -SpawnChunksSize; z < SpawnChunksSize; z++) {
				LoadChunk(x, y, z);
			}
		}
	}
	getLogger()->LogInfo("World", "Initialized World");
	WorldMainThread = std::thread(&World::WorldLoop, this);
}

void World::WorldLoop() {
	getLogger()->LogInfo("World","Started World Loop");
	while (!stop) {
		auto time0 = std::chrono::high_resolution_clock::now();
		
		DoQueuedTasks();

		double time1 = ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
		if (time1 < (1.0 / (double)TPS))
			timerSleep((1.0 / (double)TPS) - time1);
		getLogger()->LogDebug("World Tick", std::to_string(1000000000.0 / (std::chrono::high_resolution_clock::now() - time0).count()));
		
	}
}
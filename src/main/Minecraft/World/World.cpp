#include "World.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
void World::StartGenThreads(int amount) {
	for (int i = 0; i < amount; i++) {
		AddWorldGenWorker();
	}
}

void World::Initialize() {
	//Load 10x10x10 Chunks
	for (int x = -SpawnChunksSize; x < SpawnChunksSize; x++) {
		for (int y = 0; y < 16; y++) {
			for (int z = -SpawnChunksSize; z < SpawnChunksSize; z++) {
				LoadChunk(getChunkID(x, y, z));
			}
		}
	}
	getLogger()->LogInfo("World", "Initialized World");
	WorldMainThread = std::thread(&World::WorldLoop, this);
	WorldStatsThread = std::thread(&World::WorldStats, this);
}

void World::WorldLoop() {
	getLogger()->LogInfo("World","Started World Loop");
	while (!stop) {
		auto time0 = std::chrono::high_resolution_clock::now();
		
		DoQueuedTasks();
		SendChunkDataToClient();
		 
		double time1 = ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
		if (time1 < (1.0 / (double)TPS))
			timerSleepNotPrecise(((1.0 / (double)TPS) - time1)*1000);
		getLogger()->LogDebug("World Tick", std::to_string(1000000000.0 / (std::chrono::high_resolution_clock::now() - time0).count()));
		
	}
}

void World::SendChunkDataToClient() {
	for (const auto& Client : PlayerAddress.HashMap) {
		if (ClientChunkToUpdate.count(Client.first)) {
			if (ClientChunkToUpdate[Client.first].size() != 0) {
				for (const auto& chunk : ClientChunkToUpdate[Client.first]) {
					Client.second->AddChunkServer(ReadChunkMapLoaded(chunk));
				}
				ClientChunkToUpdate[Client.first].clear();
			}
		//	getLogger()->LogDebug("World", "Size_: " + std::to_string(ClientChunkToUpdate.size()));
		}
	}
}
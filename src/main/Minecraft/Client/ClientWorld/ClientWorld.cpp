#include "ClientWorld.h"
#include "../../../Utils/Clock.h"
#include "../../../Utils/LogUtils.h"
#include "../../../Utils/MathHelper.h"

void ClientWorld::AddChunkServer(Chunk chunk) {
	ChunkAddQueue.push_back(chunk);
}

void ClientWorld::Start() {
	ClientWorldThread = std::thread(&ClientWorld::ClientWorldMainLoop, this);
}

void ClientWorld::UpdateChunks() {
	while (!ChunkAddQueue.empty()) {
		Chunk chunk = ChunkAddQueue.pop_get_front();
		ChunkCache.insert(getChunkID(chunk.pos), chunk);
	}
}

void ClientWorld::ClientWorldMainLoop() {
	while (!stop) {
		auto time0 = std::chrono::high_resolution_clock::now();
		
		UpdateChunks();

		double time1 = ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
		if (time1 < (1.0 / (double)TPS))
			timerSleep((1.0 / (double)TPS) - time1);
		getLogger()->LogDebug("Client Tick", std::to_string(1000000000.0 / (std::chrono::high_resolution_clock::now() - time0).count()));
	}
}
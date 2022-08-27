#include "WorldContainer.h"
#include "Entity/Entities/Player.h"
#include "../../Utils/Clock.h"
#include "../../Utils/NumGen.h"

void WorldContainer::LoadChunk(int x, int y, int z) {
	CHUNK_ID ChunkID = getChunkID(x, y, z);

	if (CheckChunkIsStored(x,y,z)) { // Check is chunk is stored
		ChunkMapLoaded[ChunkID] = ChunkMapStore[ChunkID]; // Copy Chunk To Loaded Cache
		ChunkMapLoaded[ChunkID].clearNeighbors();
		if (ChunkMapLoaded.count(getChunkID(x, y + 1, z))) {
			ChunkMapLoaded[getChunkID(x, y + 1, z)].setNeighborNY(&ChunkMapLoaded[getChunkID(x, y, z)]);
			ChunkMapLoaded[getChunkID(x, y, z)].setNeighborPY(&ChunkMapLoaded[getChunkID(x, y + 1, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y - 1, z))) {
			ChunkMapLoaded[getChunkID(x, y - 1, z)].setNeighborPY(&ChunkMapLoaded[getChunkID(x, y, z)]);
			ChunkMapLoaded[getChunkID(x, y, z)].setNeighborNY(&ChunkMapLoaded[getChunkID(x, y - 1, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x + 1, y, z))) {
			ChunkMapLoaded[getChunkID(x + 1, y, z)].setNeighborNX(&ChunkMapLoaded[getChunkID(x, y, z)]);
			ChunkMapLoaded[getChunkID(x, y, z)].setNeighborPX(&ChunkMapLoaded[getChunkID(x + 1, y, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x - 1, y, z))) {
			ChunkMapLoaded[getChunkID(x - 1, y, z)].setNeighborPX(&ChunkMapLoaded[getChunkID(x, y, z)]);
			ChunkMapLoaded[getChunkID(x, y, z)].setNeighborNX(&ChunkMapLoaded[getChunkID(x - 1, y, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z + 1))) {
			ChunkMapLoaded[getChunkID(x, y, z + 1)].setNeighborNZ(&ChunkMapLoaded[getChunkID(x, y, z)]);
			ChunkMapLoaded[getChunkID(x, y, z)].setNeighborPZ(&ChunkMapLoaded[getChunkID(x, y, z + 1)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z - 1))) {
			ChunkMapLoaded[getChunkID(x, y, z - 1)].setNeighborPZ(&ChunkMapLoaded[getChunkID(x, y, z)]);
			ChunkMapLoaded[getChunkID(x, y, z)].setNeighborNZ(&ChunkMapLoaded[getChunkID(x, y, z - 1)]);
		}
		return;
	} 
	ChunkGenQueueMutex.lock();
	if (!ChunkProcessing.count(getChunkID(x, y, z))) {
		ChunkGenQueue.emplace_back(glm::vec3(x, y, z)); // Add chunk to gen queue
		ChunkProcessing[getChunkID(x, y, z)] = true;
	}
	ChunkGenQueueMutex.unlock();
	ChunkLoadQueue.emplace_back(glm::vec3(x, y, z)); // Add chunk to load queue
}

void WorldContainer::DoQueuedTasks() {
	while (!ChunkLoadQueue.empty()) {
		LoadChunk(ChunkLoadQueue.front().x, ChunkLoadQueue.front().y, ChunkLoadQueue.front().z);
		ChunkLoadQueue.pop_front();
	}
}

void WorldContainer::UnloadChunk(int x, int y, int z) {
	if (CheckChunkIsLoaded(x, y, z)) { // Check is chunk is loaded
		ChunkMapLoaded.erase(getChunkID(x, y, z));
		if (ChunkMapLoaded.count(getChunkID(x, y + 1, z))) {
			ChunkMapLoaded[getChunkID(x, y + 1, z)].setNeighborNY(nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y - 1, z))) {
			ChunkMapLoaded[getChunkID(x, y - 1, z)].setNeighborPY(nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x + 1, y, z))) {
			ChunkMapLoaded[getChunkID(x + 1, y, z)].setNeighborNX(nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x - 1, y, z))) {
			ChunkMapLoaded[getChunkID(x - 1, y, z)].setNeighborPX(nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z + 1))) {
			ChunkMapLoaded[getChunkID(x, y, z + 1)].setNeighborNZ(nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z - 1))) {
			ChunkMapLoaded[getChunkID(x, y, z - 1)].setNeighborPZ(nullptr);
		}
	}
}

bool WorldContainer::CheckChunkIsStored(int x, int y, int z) {
	ChunkStorageMutex.lock();
	if (ChunkMapStore.count(getChunkID(x, y, z)) == true) {
		ChunkStorageMutex.unlock();
		return true;
		
	}

	ChunkStorageMutex.unlock();
	return false;
}

bool WorldContainer::CheckChunkIsLoaded(int x, int y, int z) {
	if (ChunkMapLoaded.count(getChunkID(x, y, z)) == true)
		return true;
	return false;
}

void WorldContainer::AddWorldGenWorker() {
	WorldGenWorkers.push_back(std::thread(&WorldContainer::WorldGenerator, this));
}
void WorldContainer::DeleteWorldGenWorker() {

}
//void WorldContainer::Initialize() {
////	WorldGenWorkers.resize(127); // Max workers amount
//}

void WorldContainer::WorldGenerator() {

	getLogger()->LogInfo("World", "Started World Gen Thread");

	glm::vec3 ChunkPos;
	
	while (true) {
		while (ChunkGenQueue.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(12));
		}

		ChunkGenQueueMutex.lock();
		ChunkPos = ChunkGenQueue.front();
		ChunkGenQueue.pop_front();
		ChunkGenQueueMutex.unlock();

		Chunk chunk;
		chunk.pos = ChunkPos;
		chunk.gen_chunk(&noise);
		WriteChunkMapStore(chunk);

		ChunkGenQueueMutex.lock();
		ChunkProcessing.erase(getChunkID(ChunkPos));
		ChunkGenQueueMutex.unlock();
	}
}

void WorldContainer::WriteChunkMapStore(Chunk chunk) {
	//while (ChunkStorageBusy) {}

	ChunkStorageMutex.lock();

	ChunkMapStore[getChunkID(chunk.pos)] = chunk;

	ChunkStorageMutex.unlock();
}

Chunk WorldContainer::ReadChunkMapStore(int x, int y, int z) {
	//while (ChunkStorageBusy) {}

	Chunk chunk;

	ChunkStorageMutex.lock();

	chunk = ChunkMapStore[getChunkID(x, y, z)];

	ChunkStorageMutex.unlock();
	return chunk;

}

void WorldContainer::UpdatePlayerPosition(int Player_ID, int x, int y, int z) {
	EntityList[Player_ID]->PosX = x;
	EntityList[Player_ID]->PosX = y;
	EntityList[Player_ID]->PosX = z;
}

int WorldContainer::JoinWorld(std::string name) {
	Entity* PLAYER = new Player;
	PLAYER->PosX = 0;
	PLAYER->PosY = 100;
	PLAYER->PosZ = 0;
	PLAYER->EntityID = getID();
	PlayerList[PLAYER->EntityID] = name;
	AddEntity(PLAYER);
	return PLAYER->EntityID;
}

void WorldContainer::AddEntity(Entity* Entity) {
	EntityList[Entity->EntityID] = Entity
}
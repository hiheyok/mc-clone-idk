#include "WorldContainer.h"
#include "Entity/Entities/Player.h"
#include "../../Utils/Clock.h"
#include "../../Utils/NumGen.h"
#include "Level/Chunk/Chunk.h"
#include "../../utils/FastNoiseLite.h"
#include "../../utils/MathHelper.h"
#include "../../utils/Clock.h"
#include "../../utils/LogUtils.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
void WorldContainer::Initialize() {

}

void WorldContainer::LoadChunk(int x, int y, int z) {
	CHUNK_ID ChunkID = getChunkID(x, y, z);

	if (CheckChunkIsStored(x,y,z)) { // Check is chunk is stored
		ChunkMapLoaded[ChunkID] = ChunkMapStore[ChunkID]; // Copy Chunk To Loaded Cache
		ChunkMapLoaded[ChunkID].clearNeighbors();
		if (ChunkMapLoaded.count(getChunkID(x, y + 1, z))) {
			ChunkMapLoaded[getChunkID(x, y + 1, z)].setNeighborNY(&ChunkMapLoaded[ChunkID]);
			ChunkMapLoaded[ChunkID].setNeighborPY(&ChunkMapLoaded[getChunkID(x, y + 1, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y - 1, z))) {
			ChunkMapLoaded[getChunkID(x, y - 1, z)].setNeighborPY(&ChunkMapLoaded[ChunkID]);
			ChunkMapLoaded[ChunkID].setNeighborNY(&ChunkMapLoaded[getChunkID(x, y - 1, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x + 1, y, z))) {
			ChunkMapLoaded[getChunkID(x + 1, y, z)].setNeighborNX(&ChunkMapLoaded[ChunkID]);
			ChunkMapLoaded[ChunkID].setNeighborPX(&ChunkMapLoaded[getChunkID(x + 1, y, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x - 1, y, z))) {
			ChunkMapLoaded[getChunkID(x - 1, y, z)].setNeighborPX(&ChunkMapLoaded[ChunkID]);
			ChunkMapLoaded[ChunkID].setNeighborNX(&ChunkMapLoaded[getChunkID(x - 1, y, z)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z + 1))) {
			ChunkMapLoaded[getChunkID(x, y, z + 1)].setNeighborNZ(&ChunkMapLoaded[ChunkID]);
			ChunkMapLoaded[ChunkID].setNeighborPZ(&ChunkMapLoaded[getChunkID(x, y, z + 1)]);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z - 1))) {
			ChunkMapLoaded[getChunkID(x, y, z - 1)].setNeighborPZ(&ChunkMapLoaded[ChunkID]);
			ChunkMapLoaded[ChunkID].setNeighborNZ(&ChunkMapLoaded[getChunkID(x, y, z - 1)]);
		}

		Concurrency::concurrent_unordered_map<long long int, std::string> PLAYERS = PlayerList.DumpData();

		for (auto& players : PLAYERS) {
			Entity Player = EntityList[players.first];
			long long int PlayerID = Player.EntityID;

			if (FindDistanceNoSqrt(Player.PosX * 0.0625, Player.PosX * 0.0625, Player.PosX * 0.0625, x,y,z)  < TickingDistance * TickingDistance) {
				ClientChunkToUpdate[PlayerID].insert(ChunkID);
			}

		}
		
		return;
	}
	else {
		mut.lock();
		if (!ChunkProcessing.count(getChunkID(x, y, z))) {
			ChunkProcessing.insert(getChunkID(x, y, z));
			mut.unlock();
			ChunkGenQueue.push(getChunkID(x, y, z)); // Add chunk to gen queue
		}
		else {
			mut.unlock();
		}
		
		ChunkLoadQueue.emplace_back(glm::vec3(x, y, z)); // Add chunk to load queue
		return;
	}
	
	
}

void WorldContainer::AddChunkToGenQueue(int x, int y, int z) {

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
	if (ChunkMapStore.count(getChunkID(x, y, z)) == true)
		return true;
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

void WorldContainer::WorldGenerator() {

	getLogger()->LogInfo("World", "Started World Gen Thread");

	long long unsigned ChunkID = 0;
	
	while (true) {
		while (ChunkGenQueue.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(12)); //testt
		}

		if (ChunkGenQueue.try_pop(ChunkID)) {
			Chunk chunk;
			chunk.pos = ChunkIDToPOS(ChunkID);
			chunk.gen_chunk(&noise);
			//chunk.gen_chunkFlat();
			WriteChunkMapStore(chunk);
			mut.lock();
			ChunkProcessing.erase(ChunkID);
			mut.unlock();
		}
		
		
	}
}

void WorldContainer::WriteChunkMapStore(Chunk& chunk) {
	ChunkMapStore[getChunkID(chunk.pos)] = chunk;
}

Chunk& WorldContainer::ReadChunkMapStore(int x, int y, int z) {
	return ChunkMapStore[getChunkID(x, y, z)];
}

Chunk& WorldContainer::ReadChunkMapLoaded(int x, int y, int z) {
	return ChunkMapLoaded[getChunkID(x, y, z)];
}


Chunk& WorldContainer::ReadChunkMapStore(long long int id) {
	return ChunkMapStore[id];
}

Chunk& WorldContainer::ReadChunkMapLoaded(long long int id) {
	return ChunkMapLoaded[id];
}


void WorldContainer::UpdatePlayerPosition(int Player_ID, int x, int y, int z) {

	EntityList[Player_ID].PosX = (double)x;
	EntityList[Player_ID].PosY = (double)y;
	EntityList[Player_ID].PosZ = (double)z;

}


void WorldContainer::JoinWorld(std::string name, ClientWorld* ClientAddress) {
	Entity PLAYER;
	PLAYER.PosX = 0;
	PLAYER.PosY = 0;
	PLAYER.PosZ = 0;
	PLAYER.EntityID = getID();
	PlayerList[PLAYER.EntityID] = name;
	PlayerAddress[PLAYER.EntityID] = ClientAddress;
	Concurrency::concurrent_unordered_set<CHUNK_ID> ChunkIDCache;
	ClientChunkToUpdate[PLAYER.EntityID] = ChunkIDCache;
	AddEntity(PLAYER);

	int cx = (int)(PLAYER.PosX / 16.0f);
	int cy = (int)(PLAYER.PosY / 16.0f);
	int cz = (int)(PLAYER.PosZ / 16.0f);

	for (int x = -TickingDistance + cx; x < TickingDistance + cx; x++) {
		for (int y = 0 + cy; y < 16 + cy; y++) {
			for (int z = -TickingDistance + cz; z < TickingDistance + cz; z++) {
				if (CheckChunkIsStored(x,y,z)) {
					if (CheckChunkIsLoaded(x, y, z)) {
						ClientAddress->AddChunkServer(ReadChunkMapLoaded(x, y, z));
					}
					else {
						ClientAddress->AddChunkServer(ReadChunkMapStore(x, y, z));

						LoadChunk(x,y,z);
					}
				}
				else {
					mut.lock();
					if (!ChunkProcessing.count(getChunkID(x, y, z))) {
						ChunkProcessing.insert(getChunkID(x, y, z));
						mut.unlock();
						ChunkGenQueue.push(getChunkID(x, y, z)); // Add chunk to gen queue
					}
					else {
						mut.unlock();
					}
					
					ChunkLoadQueue.emplace_back(glm::vec3(x, y, z)); // Add chunk to load queue
				}
			}
		}
	}
	//return PLAYER->EntityID;
}

void WorldContainer::AddEntity(Entity Entity) {
	EntityList[Entity.EntityID] = Entity;
}

void WorldContainer::WorldStats() {
	size_t p0 = 0;
	size_t p1 = 1;

	while (true) {

		p0 = ChunkMapStore.size();
		timerSleepNotPrecise(1000);
		p0 = ChunkMapStore.size() - p0;
		if (p0 != 0) {
			getLogger()->LogInfo("World", "Chunk Gen Rate: " + std::to_string(p0));
		}
	}
}
#include "WorldContainer.h"
#include "Entity/Entities/Player.h"
#include "../../Utils/Clock.h"
#include "../../Utils/NumGen.h"
#include "Level/Chunk/Chunk.h"
#include "../../utils/FastNoiseLite.h"
#include "../../utils/MathHelper.h"
#include "../../utils/Clock.h"
#include "../../utils/LogUtils.h"

void WorldContainer::LoadChunk(int x, int y, int z) {
	CHUNK_ID ChunkID = getChunkID(x, y, z);

	if (CheckChunkIsStored(x,y,z)) { // Check is chunk is stored
		ChunkMapLoaded.insert(ChunkID, ChunkMapStore.get(ChunkID)); // Copy Chunk To Loaded Cache
		ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::clearNeighbors);
		if (ChunkMapLoaded.count(getChunkID(x, y + 1, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y + 1, z), &Chunk::setNeighborNY, ChunkMapLoaded.getAddress(ChunkID));
			ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::setNeighborPY,ChunkMapLoaded.getAddress(getChunkID(x, y + 1, z)));
		}
		if (ChunkMapLoaded.count(getChunkID(x, y - 1, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y - 1, z), &Chunk::setNeighborPY,ChunkMapLoaded.getAddress(ChunkID));
			ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::setNeighborNY,ChunkMapLoaded.getAddress(getChunkID(x, y - 1, z)));
		}
		if (ChunkMapLoaded.count(getChunkID(x + 1, y, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x + 1, y, z), &Chunk::setNeighborNX,ChunkMapLoaded.getAddress(ChunkID));
			ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::setNeighborPX,ChunkMapLoaded.getAddress(getChunkID(x + 1, y, z)));
		}
		if (ChunkMapLoaded.count(getChunkID(x - 1, y, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x - 1, y, z), &Chunk::setNeighborPX,ChunkMapLoaded.getAddress(ChunkID));
			ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::setNeighborNX,ChunkMapLoaded.getAddress(getChunkID(x - 1, y, z)));
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z + 1))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y, z + 1), &Chunk::setNeighborNZ,ChunkMapLoaded.getAddress(ChunkID));
			ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::setNeighborPZ,ChunkMapLoaded.getAddress(getChunkID(x, y, z + 1)));
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z - 1))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y, z - 1), &Chunk::setNeighborPZ,ChunkMapLoaded.getAddress(ChunkID));
			ChunkMapLoaded.RunObjFunction(ChunkID, &Chunk::setNeighborNZ,ChunkMapLoaded.getAddress(getChunkID(x, y, z - 1)));
		}
		ClientChunkToUpdate.insert(ChunkID, ChunkMapLoaded.get(ChunkID));
		return;
	}
	else {
		if (!ChunkProcessing.count(getChunkID(x, y, z))) {
			ChunkGenQueue.emplace_back(glm::vec3(x, y, z)); // Add chunk to gen queue
			ChunkProcessing.insert(getChunkID(x, y, z), true);
		}
		ChunkLoadQueue.emplace_back(glm::vec3(x, y, z)); // Add chunk to load queue
		return;
	}
	
	
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
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y + 1, z), &Chunk::setNeighborNY, nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y - 1, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y - 1, z), &Chunk::setNeighborPY,nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x + 1, y, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x + 1, y, z), &Chunk::setNeighborNX,nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x - 1, y, z))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x - 1, y, z), &Chunk::setNeighborPX,nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z + 1))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y, z + 1), &Chunk::setNeighborNZ,nullptr);
		}
		if (ChunkMapLoaded.count(getChunkID(x, y, z - 1))) {
			ChunkMapLoaded.RunObjFunction(getChunkID(x, y, z - 1), &Chunk::setNeighborPZ,nullptr);
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

	glm::vec3 ChunkPos;
	
	while (true) {
		while (ChunkGenQueue.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(12)); //testt
		}

		ChunkPos = ChunkGenQueue.pop_get_front();

		Chunk chunk;
		chunk.pos = ChunkPos;
		chunk.gen_chunk(&noise);
		WriteChunkMapStore(chunk);

		ChunkProcessing.erase(getChunkID(ChunkPos));
	}
}

void WorldContainer::WriteChunkMapStore(Chunk chunk) {
	ChunkMapStore.insert(getChunkID(chunk.pos), chunk);
}

Chunk WorldContainer::ReadChunkMapStore(int x, int y, int z) {
	return ChunkMapStore.get(getChunkID(x, y, z));

}



void WorldContainer::UpdatePlayerPosition(int Player_ID, int x, int y, int z) {

	EntityList.ChangeObjMember(Player_ID, &Entity::PosX, (double)x);
	EntityList.ChangeObjMember(Player_ID, &Entity::PosY, (double)y);
	EntityList.ChangeObjMember(Player_ID, &Entity::PosZ, (double)z);

}


void WorldContainer::JoinWorld(std::string name, ClientWorld* ClientAddress) {
	Entity PLAYER;
	PLAYER.PosX = 0;
	PLAYER.PosY = 0;
	PLAYER.PosZ = 0;
	PLAYER.EntityID = getID();
	PlayerList.insert(PLAYER.EntityID, name);
	PlayerAddress.insert(PLAYER.EntityID, ClientAddress);
	AddEntity(PLAYER);
	//return PLAYER->EntityID;
}

void WorldContainer::AddEntity(Entity Entity) {
	EntityList.insert(Entity.EntityID, Entity);
}


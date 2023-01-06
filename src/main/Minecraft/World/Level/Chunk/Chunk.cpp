#include "Chunk.h"

BlockID Chunk::getBlock(int x, int y, int z) {

	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		if (x < 0)
			return GetNeighborPointer(NX) != nullptr ? neighbors[3]->getBlock(x + CHUNK_SIZE, y, z) : AIR;
		if (x >= CHUNK_SIZE)
			return GetNeighborPointer(PX) != nullptr ? neighbors[2]->getBlock(x - CHUNK_SIZE, y, z) : AIR;
		if (y < 0)
			return GetNeighborPointer(NY) != nullptr ? neighbors[1]->getBlock(x, y + CHUNK_SIZE, z) : AIR;
		if (y >= CHUNK_SIZE)
			return GetNeighborPointer(PY) != nullptr ? neighbors[0]->getBlock(x, y - CHUNK_SIZE, z) : AIR;
		if (z < 0)
			return GetNeighborPointer(NZ) != nullptr ? neighbors[5]->getBlock(x, y, z + CHUNK_SIZE) : AIR;
		if (z >= CHUNK_SIZE)
			return GetNeighborPointer(PZ) != nullptr ? neighbors[4]->getBlock(x, y, z - CHUNK_SIZE) : AIR;
	}
	else {
		return data[(x * 256) + (z * 16) + y];
	}
}



void Chunk::clearNeighbors() {
	neighbors[0] = nullptr;
	neighbors[1] = nullptr;
	neighbors[2] = nullptr;
	neighbors[3] = nullptr;
	neighbors[4] = nullptr;
	neighbors[5] = nullptr;
}

void Chunk::NeighborEntityTransfer(int x, int y, int z, Entity* entity) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		if (x < 0 && (GetNeighborPointer(NX) != nullptr)) {
			GetNeighborPointer(NX)->SetQueuedNeighborEntity(x + CHUNK_SIZE, y, z, entity);
			EntityList.erase(entity->EntityUUID);
			return;
		}
			
		if (x >= CHUNK_SIZE && (GetNeighborPointer(PX) != nullptr)) {
			GetNeighborPointer(PX)->SetQueuedNeighborEntity(x - CHUNK_SIZE, y, z, entity);
			EntityList.erase(entity->EntityUUID);
			return;
		}
			
		if (y < 0 && (GetNeighborPointer(NY) != nullptr)) {
			GetNeighborPointer(NY)->SetQueuedNeighborEntity(x, y + CHUNK_SIZE, z, entity);
			EntityList.erase(entity->EntityUUID);
			return;
		}
			
		if (y >= CHUNK_SIZE && (GetNeighborPointer(PY) != nullptr)) {
			GetNeighborPointer(PY)->SetQueuedNeighborEntity(x, y - CHUNK_SIZE, z, entity);
			EntityList.erase(entity->EntityUUID);
			return;
		}
			
		if (z < 0 && (GetNeighborPointer(NZ) != nullptr)) {
			GetNeighborPointer(NZ)->SetQueuedNeighborEntity(x, y, z + CHUNK_SIZE, entity);
			EntityList.erase(entity->EntityUUID);
			return;
		}
			
		if (z >= CHUNK_SIZE && (GetNeighborPointer(PZ) != nullptr)) {
			GetNeighborPointer(PZ)->SetQueuedNeighborEntity(x, y, z - CHUNK_SIZE, entity);
			EntityList.erase(entity->EntityUUID);
			return;
		}
		getLogger()->LogInfo("Chunk", "Can't transfer entity. Chunk does not exist | UUID: " + std::to_string(entity->EntityUUID));
		return;
	}
	else {
		getLogger()->LogWarn("Chunk", "Can't transfer entity. Already in expected chunk | UUID: " + std::to_string(entity->EntityUUID));
		return;
	}
}

void Chunk::SetQueuedNeighborEntity(int x, int y, int z, Entity* entity) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		NeighborEntityTransfer(x, y, z, entity);
		getLogger()->LogDebug("Chunk", "Retransfering Entity | UUID: " + std::to_string(entity->EntityUUID));
	}
	else {
		getLogger()->LogDebug("Chunk", "Transfering Entity Success | UUID: " + std::to_string(entity->EntityUUID));
		EntityUpdateQueue.push_back(entity);
	}
	
}

void Chunk::SetNeighborPointer(unsigned char Direction, Chunk* chunk) {
	neighbors[Direction] = chunk;
}

Chunk* Chunk::GetNeighborPointer(unsigned char Direction) {
	return neighbors[Direction];
}
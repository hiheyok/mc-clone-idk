#pragma once
#ifndef CHUNK_H_
#define CHUNK_H_

#include "ChunkProperties.h"
#include "ChunkData.h"
#include "../../../../Utils/Math/ChunkID/ChunkID.h"

constexpr unsigned char PY = 0;
constexpr unsigned char NY = 1;
constexpr unsigned char PX = 2;
constexpr unsigned char NX = 3;
constexpr unsigned char PZ = 4;
constexpr unsigned char NZ = 5;

struct BlockUpdate {
	unsigned char type = NULL;
	BlockID BlockType = NULL;
	ivec3 LocalPosition;
};

class Chunk : public ChunkProperties, ChunkData {
public:
	Chunk(int x, int y, int z) {
		ChunkID = getChunkID(x,y,z);
	}
	
	BlockID getBlock(int x, int y, int z);
	
protected:

	void NeighborEntityTransfer(int x, int y, int z, Entity* entity);
	void SetQueuedNeighborEntity(int x, int y, int z, Entity* entity);

private:

	void EntityTick();
	void BlockTick();

	void SetNeighborPointer(unsigned char Direction, Chunk* chunk);
	Chunk* GetNeighborPointer(unsigned char Direction);
	void clearNeighbors();
	std::deque<BlockUpdate> BlockUpdateQueue;
	std::deque<Entity*> EntityUpdateQueue;
	
	Chunk* neighbors[6]{ nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
	
	

};

#endif
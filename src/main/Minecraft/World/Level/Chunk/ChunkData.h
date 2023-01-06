#pragma once
#include "../Block/Blocks.h"
#include "../../../Core/Registry.h"
#include "../../../../Utils/Math/ChunkID/ChunkID.h"

constexpr auto CHUNK_SIZE = 16;
constexpr auto CHUNK_SIZE_2 = CHUNK_SIZE * CHUNK_SIZE;
constexpr auto CHUNK_SIZE_3 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;



typedef long long unsigned int CHUNK_ID;

struct ChunkData {
	ChunkData() {

	}

	void addBlock(int x, int y, int z, BlockID type);
	
	void clearChunk();

	CHUNK_ID ChunkID = NULL;
	std::unordered_map<UUID, Entity*> EntityList;
	BlockID data[4096]{ AIR };
	
};
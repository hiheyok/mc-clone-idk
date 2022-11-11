#pragma once
#include "Chunk.h"
#include <unordered_map>
class ChunkMap {
	
	BlockID getBlock(CHUNK_ID ChunkID, int x, int y, int z) {
		return checkChunk(ChunkID) ? ChunkStorage[ChunkID].getBlock(x, y, z) : AIR;
	}

	bool checkChunk(CHUNK_ID ChunkID) {
		return ChunkStorage.count(ChunkID);
	}

	Chunk& operator[](CHUNK_ID ChunkID) {
		return ChunkStorage[ChunkID];
	}
private:
	std::unordered_map<CHUNK_ID, Chunk> ChunkStorage;
};
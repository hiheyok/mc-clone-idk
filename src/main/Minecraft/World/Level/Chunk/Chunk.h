#ifndef CHUNK_H_
#define CHUNK_H_

#include "../Block/Block.h"

typedef long long unsigned int CHUNK_ID;

constexpr auto CHUNK_SIZE = 16;
constexpr auto SEED = 3453454;

constexpr auto CHUNK_SIZE_2 = CHUNK_SIZE * CHUNK_SIZE;
constexpr auto CHUNK_SIZE_3 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;


#include "../Block/Blocks.h"
#include "../../../Core/Registry.h"
#include "../../../../Utils/Math/ChunkID/ChunkID.h"

class Chunk {
public:
	Chunk(int x, int y, int z) {
		ChunkID = getChunkID(x,y,z);
	}
	void addBlock(int x, int y, int z, BlockID type);

	BlockID getBlock(int x, int y, int z);

	void clearChunk() {
		for (int i = 0; i < 16 * 16 * 16; i++) {
			data[i] = AIR;
		}
	}

	void setNeighborPY(Chunk* chunk) {
		neighbors[0] = chunk;
	}
	void setNeighborNY(Chunk* chunk) {
		neighbors[1] = chunk;
	}
	void setNeighborPX(Chunk* chunk) {
		neighbors[2] = chunk;
	}
	void setNeighborNX(Chunk* chunk) {
		neighbors[3] = chunk;
	}
	void setNeighborPZ(Chunk* chunk) {
		neighbors[4] = chunk;
	}
	void setNeighborNZ(Chunk* chunk) {
		neighbors[5] = chunk;
	}

	Chunk* getNeighborAddressPY() {
		return neighbors[0];
	}
	Chunk* getNeighborAddressNY() {
		return neighbors[1];
	}
	Chunk* getNeighborAddressPX() {
		return neighbors[2];
	}
	Chunk* getNeighborAddressNX() {
		return neighbors[3];
	}
	Chunk* getNeighborAddressPZ() {
		return neighbors[4];
	}
	Chunk* getNeighborAddressNZ() {
		return neighbors[5];
	}

	void clearNeighbors() {
		neighbors[0] = nullptr;
		neighbors[1] = nullptr;
		neighbors[2] = nullptr;
		neighbors[3] = nullptr;
		neighbors[4] = nullptr;
		neighbors[5] = nullptr;
	}

	CHUNK_ID ChunkID = NULL;
private:

	BlockID data[4096]{};
	bool empty = true;
	Chunk* neighbors[6]{ nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

};

#endif
#ifndef CHUNK_H_
#define CHUNK_H_

typedef long long int CHUNK_ID;

constexpr auto CHUNK_SIZE = 16;
constexpr auto SEED = 3453454;

constexpr auto CHUNK_SIZE_2 = CHUNK_SIZE * CHUNK_SIZE;
constexpr auto CHUNK_SIZE_3 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

#define AIR 0x00
#define GRASS 0x01
#define DIRT 0x02
#define COBBLESTONE 0x03
#define STONE 0x04
#define SAND 0x05
#define WATER 0x06
#define OAK_LOG 0x07
#define OAK_LEAF 0x08

#define ID 0x00
#define LIGHT 0x01

#define GENERATED true



#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "../../../../utils/FastNoiseLite.h"

unsigned int extractBlockData(char type, unsigned int data);

class Block {
public:
	char id = AIR;
	bool transparent() {
		if (id == AIR)
			return true;
		if (id == WATER)
			return true;
		//false
		if (id == GRASS)
			return false;
		if (id == DIRT)
			return false;
		if (id == COBBLESTONE)
			return false;
		if (id == STONE)
			return false;
		if (id == SAND)
			return false;
		if (id == OAK_LEAF)
			return true;
		if (id == OAK_LOG)
			return false;
		return false;
	}
};

class Chunk {
public:
	Chunk() {
		null_block.id = 0;
	}
	void addblock(int x, int y, int z, unsigned char type);

	bool isEmpty() {
		return empty;
	}

	void setPOS(int x, int y, int z) {
		pos = glm::ivec4(x, y, z, 0);
	}

	glm::ivec3 getcpos() {
		return glm::ivec3(pos.x, pos.y, pos.z);
	}

	glm::ivec3 getcposivec4() {
		return pos;
	}

	Block checkblock(int x, int y, int z);

	void gen_chunkFlat();
	void gen_chunkTerrain(FastNoiseLite* noise);
	void gen_chunkPerlinCave(FastNoiseLite* noise);
	Block* getChunKData() {

		return data;
	}

	void gen_chunk(FastNoiseLite* noise);
	void gen_chunkSphere();

	long long int getChunkMEMSize() {
		return sizeof(data);
	}

	bool checkBlockTransparency(unsigned char type);

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

	Chunk getNeighborPY() {
		return *neighbors[0];
	}
	Chunk getNeighborNY() {
		return *neighbors[1];
	}
	Chunk getNeighborPX() {
		return *neighbors[2];
	}
	Chunk getNeighborNX() {
		return *neighbors[3];
	}
	Chunk getNeighborPZ() {
		return *neighbors[4];
	}
	Chunk getNeighborNZ() {
		return *neighbors[5];
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
		neighborsSet = false;
		neighbors[0] = nullptr;
		neighbors[1] = nullptr;
		neighbors[2] = nullptr;
		neighbors[3] = nullptr;
		neighbors[4] = nullptr;
		neighbors[5] = nullptr;
	}

	bool RenderReady = false;

	bool init = NULL;

	Block null_block;
	glm::ivec3 pos = glm::ivec3(0, 0, 0);
private:
	Block data[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE]{};
	
	bool empty = true;
	Chunk* neighbors[6]{ nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
	bool neighborsSet = false;
};

Chunk GenerateChunk(int x, int y, int z);
#endif
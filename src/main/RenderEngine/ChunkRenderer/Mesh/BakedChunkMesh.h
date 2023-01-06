#pragma once

#include "../../../Minecraft/World/Level/Chunk/Chunk.h"

struct BlockVerticesData {
	BlockVerticesData(BlockID id = AIR) : data(id) {}
	unsigned int Vdata[6]{};
	BlockID data;
};

struct BakedChunkData {
	void addUninitBlock(int x, int y, int z, char B_ID);
	BlockVerticesData extract(int x, int y, int z);
	BlockID GetBlockID(int x, int y, int z);
	bool compare(int x, int y, int z, int x1, int y1, int z1, char type);
	bool isTransparent(int x, int y, int z);
	unsigned int extractVData(char type, unsigned int data);
	BlockVerticesData data[CHUNK_SIZE_3]{};
	void add(int x, int y, int z, char B_ID, unsigned int L0_, unsigned int L1_, unsigned int L2_, unsigned int L3_, char side);
};

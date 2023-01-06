#include "BakedChunkMesh.h"

BlockID BakedChunkData::GetBlockID(int x, int y, int z) {
	return extract(x,y,z).data;
}

BlockVerticesData BakedChunkData::extract(int x, int y, int z) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		BlockVerticesData data;
		data.data = AIR;
		return data;
	}
	else {
		return data[(x * CHUNK_SIZE_2) + (z * CHUNK_SIZE) + y];
	}
}

void BakedChunkData::add(int x, int y, int z, char B_ID, unsigned int L0_, unsigned int L1_, unsigned int L2_, unsigned int L3_, char side) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {

	}
	else {
		data[(x * CHUNK_SIZE_2) + (z * CHUNK_SIZE) + y].Vdata[side] = 0 | (B_ID) | (L0_ << 8) | (L1_ << 12) | (L2_ << 16) | (L3_ << 20);
		data[(x * CHUNK_SIZE_2) + (z * CHUNK_SIZE) + y].data = B_ID;
	}

}
void BakedChunkData::addUninitBlock(int x, int y, int z, char B_ID) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {

	}
	else {
		data[x * CHUNK_SIZE_2 + z * CHUNK_SIZE + y].data = B_ID;
	}

}
bool BakedChunkData::compare(int x, int y, int z, int x1, int y1, int z1, char type) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0 || x1 >= CHUNK_SIZE || y1 >= CHUNK_SIZE || z1 >= CHUNK_SIZE || x1 < 0 || y1 < 0 || z1 < 0)
		return false;
	unsigned int q0 = data[x * CHUNK_SIZE_2 + z * CHUNK_SIZE + y].Vdata[type];
	unsigned int q1 = data[x * CHUNK_SIZE_2 + z * CHUNK_SIZE + y].Vdata[type];
	if (
		(extractVData(BMESH_ID, q0) == extractVData(BMESH_ID, q1))
		&& (extractVData(BMESH_L0, q0) == extractVData(BMESH_L0, q1))
		&& (extractVData(BMESH_L1, q0) == extractVData(BMESH_L1, q1))
		&& (extractVData(BMESH_L2, q0) == extractVData(BMESH_L2, q1))
		&& (extractVData(BMESH_L3, q0) == extractVData(BMESH_L3, q1))
		)
		return true;
	return false;
}
unsigned int BakedChunkData::extractVData(char type, unsigned int data) {
	if (type == BMESH_ID)
		return (((1u << 8) - 1u) & (data >> 0));
	if (type == BMESH_L0)
		return (((1u << 4) - 1u) & (data >> 8));
	if (type == BMESH_L1)
		return (((1u << 4) - 1u) & (data >> 12));
	if (type == BMESH_L2)
		return (((1u << 4) - 1u) & (data >> 16));
	if (type == BMESH_L3)
		return (((1u << 4) - 1u) & (data >> 20));
	return 0;
}

bool BakedChunkData::isTransparent(int x, int y, int z) {
	return BlockRegistry[extract(x, y, z).data]->transparency;
}

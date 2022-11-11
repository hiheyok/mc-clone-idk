#include "Chunk.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

BlockID Chunk::getBlock(int x, int y, int z) {

	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		if (x < 0)
			return getNeighborAddressNX() != nullptr ? neighbors[3]->getBlock(x + CHUNK_SIZE, y, z) : AIR;
		if (x >= CHUNK_SIZE)
			return getNeighborAddressPX() != nullptr ? neighbors[2]->getBlock(x - CHUNK_SIZE, y, z) : AIR;
		if (y < 0)
			return getNeighborAddressNY() != nullptr ? neighbors[1]->getBlock(x, y + CHUNK_SIZE, z) : AIR;
		if (y >= CHUNK_SIZE)
			return getNeighborAddressPY() != nullptr ? neighbors[0]->getBlock(x, y - CHUNK_SIZE, z) : AIR;
		if (z < 0)
			return getNeighborAddressNZ() != nullptr ? neighbors[5]->getBlock(x, y, z + CHUNK_SIZE) : AIR;
		if (z >= CHUNK_SIZE)
			return getNeighborAddressPZ() != nullptr ? neighbors[4]->getBlock(x, y, z - CHUNK_SIZE) : AIR;
	}
	else {
		return data[(x * 256) + (z * 16) + y];
	}
}

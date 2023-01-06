#include "ChunkData.h"




void ChunkData::clearChunk() {
	for (int i = 0; i < 16 * 16 * 16; i++) {
		data[i] = AIR;
	}
}

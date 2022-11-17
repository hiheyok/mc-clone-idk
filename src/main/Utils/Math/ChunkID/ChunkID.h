#pragma once
#include "../Vector/ivec3.h"
#include "../FastMath.h"
static long long unsigned int getChunkID(int x, int y, int z) {
	long long unsigned int sx = (((1u << 1) - 1u) & (x >> 31));
	long long unsigned int sy = (((1u << 1) - 1u) & (y >> 31));
	long long unsigned int sz = (((1u << 1) - 1u) & (z >> 31));
	long long unsigned int ax = (long long unsigned int)FastABS(x);
	long long unsigned int ay = (long long unsigned int)FastABS(y);
	long long unsigned int az = (long long unsigned int)FastABS(z);

	return 0LLU | ax << 0 | ay << 16 | az << 32 | sx << 61 | sy << 62 | sz << 63;
}
static long long unsigned int getChunkID(ivec3 vec) {
	long long unsigned int sx = (((1u << 1) - 1u) & (vec.x >> 31));
	long long unsigned int sy = (((1u << 1) - 1u) & (vec.y >> 31));
	long long unsigned int sz = (((1u << 1) - 1u) & (vec.z >> 31));
	long long unsigned int ax = (long long unsigned int)FastABS(vec.x);
	long long unsigned int ay = (long long unsigned int)FastABS(vec.y);
	long long unsigned int az = (long long unsigned int)FastABS(vec.z);

	return 0LLU | ax << 0 | ay << 16 | az << 32 | sx << 61 | sy << 62 | sz << 63;
}

static ivec3 ChunkIDToPOS(long long unsigned int n) {
	int x = ((unsigned int)(((1u << 1) - 1u) & (n >> 61)) << 31) ? -(int)(((1LLU << 16) - 1u) & (n >> 0LLU)) : (int)(((1LLU << 16) - 1u) & (n >> 0LLU));
	int y = ((unsigned int)(((1u << 1) - 1u) & (n >> 62)) << 31) ? -(int)(((1LLU << 16) - 1u) & (n >> 16LLU)) : (int)(((1LLU << 16) - 1u) & (n >> 16LLU));
	int z = ((unsigned int)(((1u << 1) - 1u) & (n >> 63)) << 31) ? -(int)(((1LLU << 16) - 1u) & (n >> 32LLU)) : (int)(((1LLU << 16) - 1u) & (n >> 32LLU));
	return ivec3(x, y, z);
}

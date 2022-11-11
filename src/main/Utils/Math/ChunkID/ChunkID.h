#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
long long unsigned int getChunkID(int x, int y, int z) {
	long long unsigned int sx = (((1u << 1) - 1u) & (x >> 31));
	long long unsigned int sy = (((1u << 1) - 1u) & (y >> 31));
	long long unsigned int sz = (((1u << 1) - 1u) & (z >> 31));
	long long unsigned int ax = (long long unsigned int)abs(x);
	long long unsigned int ay = (long long unsigned int)abs(y);
	long long unsigned int az = (long long unsigned int)abs(z);

	return 0LLU | ax << 0 | ay << 16 | az << 32 | sx << 61 | sy << 62 | sz << 63;
}
long long unsigned int getChunkID(glm::ivec3 vec) {
	long long unsigned int sx = (((1u << 1) - 1u) & (vec.x >> 31));
	long long unsigned int sy = (((1u << 1) - 1u) & (vec.y >> 31));
	long long unsigned int sz = (((1u << 1) - 1u) & (vec.z >> 31));
	long long unsigned int ax = (long long unsigned int)abs(vec.x);
	long long unsigned int ay = (long long unsigned int)abs(vec.y);
	long long unsigned int az = (long long unsigned int)abs(vec.z);

	return 0LLU | ax << 0 | ay << 16 | az << 32 | sx << 61 | sy << 62 | sz << 63;
}

glm::ivec3 ChunkIDToPOS(long long unsigned int n) {
	int x = ((unsigned int)(((1u << 1) - 1u) & (n >> 61)) << 31) ? -(int)(((1LLU << 16) - 1u) & (n >> 0LLU)) : (int)(((1LLU << 16) - 1u) & (n >> 0LLU));
	int y = ((unsigned int)(((1u << 1) - 1u) & (n >> 62)) << 31) ? -(int)(((1LLU << 16) - 1u) & (n >> 16LLU)) : (int)(((1LLU << 16) - 1u) & (n >> 16LLU));
	int z = ((unsigned int)(((1u << 1) - 1u) & (n >> 63)) << 31) ? -(int)(((1LLU << 16) - 1u) & (n >> 32LLU)) : (int)(((1LLU << 16) - 1u) & (n >> 32LLU));
	return glm::vec3(x, y, z);
}

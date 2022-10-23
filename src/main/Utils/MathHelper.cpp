#include <tgmath.h>
#include <gl/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#define _CRTDBG_MAP_ALLOC
using namespace glm;

double toRadians(double x)
{
    return x* 0.01745329251;
}

double FindDistanceNoSqrt(double x, double y, double z, double x1, double y1, double z1) {
	return ((double)x - (double)x1) * ((double)x - (double)x1) + ((double)y - (double)y1) * ((double)y - (double)y1) + ((double)z - (double)z1) * ((double)z - (double)z1);
}

double FindDistanceNoSqrt(int x, int y, int x1, int y1) {
	return pow(x - x1, 2) + pow(y - y1, 2);
}

double FindDistance(int x, int y, int z, int x1, int y1, int z1) {
	return sqrt(pow(x - x1, 2) + pow(y - y1, 2) + pow(z - z1, 2));
}

double FindDistance(int x, int y, int x1, int y1) {
	return sqrt(pow(x - x1, 2) + pow(y - y1, 2));
}

template <typename T> double FindDistance(T x, T x1) {
	return sqrt(pow((double)x - x1, 2));
}

double FindDistance(ivec3 pos0, ivec3 pos1) {
	return sqrt(pow((double)pos0.x - pos1.x, 2) + pow((double)pos0.y - pos1.y, 2) + pow((double)pos0.z - pos1.z, 2));
}

double FindDistance(ivec2 pos0, ivec2 pos1) {
	return sqrt(pow((double)pos0.x - pos1.x, 2) + pow((double)pos0.y - pos1.y, 2));
}

double FindDistance(vec3 pos0, vec3 pos1) {
	return sqrt(pow((double)pos0.x - pos1.x, 2) + pow((double)pos0.y - pos1.y, 2) + pow((double)pos0.z - pos1.z, 2));
}

double FindDistance(vec2 pos0, vec2 pos1) {
	return sqrt(pow((double)pos0.x - pos1.x, 2) + pow((double)pos0.y - pos1.y, 2));
}

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

	int x = 0;
	int y = 0;
	int z = 0;
	if (((uint)(((1u << 1) - 1u) & (n >> 61)) << 31)) {
		x = -(int)(((1LLU << 16) - 1u) & (n >> 0LLU));
	}
	else {
		x = (int)(((1LLU << 16) - 1u) & (n >> 0LLU));
	}
	if (((uint)(((1u << 1) - 1u) & (n >> 62)) << 31)) {
		y = -(int)(((1LLU << 16) - 1u) & (n >> 16LLU));
	}
	else {
		y = (int)(((1LLU << 16) - 1u) & (n >> 16LLU));
	}
	if (((uint)(((1u << 1) - 1u) & (n >> 63)) << 31)) {
		z = -(int)(((1LLU << 16) - 1u) & (n >> 32LLU));
	}
	else {
		z = (int)(((1LLU << 16) - 1u) & (n >> 32LLU));
	}
	return glm::vec3(x,y,z);
}

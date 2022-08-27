#include <tgmath.h>
#include <gl/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;

float fastsqrt(float n) { // QUAKE 3
    n = 1.0f / n;
    long i;
    float x, y;

    x = n * 0.5f;
    y = n;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1); // ????????????????
    y = *(float*)&i;
	y = y * (1.5f - (x * y * y));

    return y;
}

double toRadians(double x)
{
    return x* 0.01745329251;
}


double FindDistance(int x, int y, int z, int x1, int y1, int z1) {
	return sqrt(pow(x - x1, 2) + pow(y - y1, 2) + pow(z - z1, 2));
}

double FindDistance(int x, int y, int x1, int y1) {
	return sqrt(pow(x - x1, 2) + pow(y - y1, 2));
}

double FindDistance(int x, int x1) {
	return sqrt(pow((double)x - x1, 2));
}
double FindDistance(float x, float x1) {
	return sqrt(pow((double)x - x1, 2));
}
double FindDistance(double x, double x1) {
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






float FastFindDistance(int x, int y, int z, int x1, int y1, int z1) {
	return fastsqrt((float)pow(x - x1, 2) + (float)pow(y - y1, 2) + (float)pow(z - z1, 2));
}

float FastFindDistance(int x, int y, int x1, int y1) {
	return fastsqrt((float)pow(x - x1, 2) + (float)pow(y - y1, 2));
}

float FastFindDistance(double x, double y, double z, double x1, double y1, double z1) {
	return fastsqrt((float)pow(x - x1, 2) + (float)pow(y - y1, 2) + (float)pow(z - z1, 2));
}

float FastFindDistance(double x, double y, double x1, double y1) {
	return fastsqrt((float)pow(x - x1, 2) + (float)pow(y - y1, 2));
}

long long int getChunkID(int x, int y, int z) {
	return ((long long int)x * 1000000000000) + ((long long int)y * 1000000) + (long long int)z;
}

long long int getChunkID(glm::ivec3 vec) {
	return ((long long int)vec.x * 1000000000000) + ((long long int)vec.y * 1000000) + (long long int)vec.z;
}

glm::ivec3 IntToIVec3(long long int n) {

	int lz = n % 1000000;
	int lx = n / (1000000000000.0);
	int ly = (n % (1000000000000)) / 1000000;

	return glm::vec3(lx,ly,lz);
}

#pragma once
#include "../FastMath.h"
struct ivec3 {
	template <typename U, typename T, typename Y>
	ivec3(U x_, T y_, Y z_) : x((int)x_), y((int)y_), z((int)z_) {};

	ivec3 operator+(ivec3 vec) {
		return ivec3(x + vec.x, y + vec.y, z + vec.z);
	}
	ivec3 operator-(ivec3 vec) {
		return ivec3(x - vec.x, y - vec.y, z - vec.z);
	}
	//vec  num
	ivec3 operator+(int a) {
		return ivec3(x + a, y + a, z + a);
	}
	ivec3 operator-(int s) {
		return ivec3(x - s, y - s, z - s);
	}
	ivec3 operator*(int m) {
		return ivec3(x * m, y * m, z * m);
	}
	ivec3 operator/(int d) {
		return ivec3(x - d, y - d, z - d);
	}
	long long unsigned int GetID()  {
		long long unsigned int sx = (((1u << 1) - 1u) & (x >> 31));
		long long unsigned int sy = (((1u << 1) - 1u) & (y >> 31));
		long long unsigned int sz = (((1u << 1) - 1u) & (z >> 31));
		long long unsigned int ax = (long long unsigned int)FastABS(x);
		long long unsigned int ay = (long long unsigned int)FastABS(y);
		long long unsigned int az = (long long unsigned int)FastABS(z);

		return 0LLU | ax << 0 | ay << 16 | az << 32 | sx << 61 | sy << 62 | sz << 63;
	}
	int x = 0, y = 0, z = 0;
};
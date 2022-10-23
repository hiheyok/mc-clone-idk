#pragma once
#ifndef BLOCK_ATLAS_H
#define BLOCK_ATLAS_H
#define _CRTDBG_MAP_ALLOC
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "../../../Minecraft/World/Level/Chunk/Chunk.h"

struct Block_UVS
{
	unsigned int UV_SIDE_PZ;
	unsigned int UV_SIDE_NZ;
	unsigned int UV_SIDE_NY;
	unsigned int UV_SIDE_PY;
	unsigned int UV_SIDE_NX;
	unsigned int UV_SIDE_PX;
};

class block_uv {
public:

	void setBlockUVS(int id) {
		if (id == GRASS) {
			

			block.UV_SIDE_NY = 1;
			block.UV_SIDE_PY = 3;
			block.UV_SIDE_PX = 2;
			block.UV_SIDE_NX = 2;
			block.UV_SIDE_PZ = 2;
			block.UV_SIDE_NZ = 2;
		}
		if (id == DIRT) {


			block.UV_SIDE_NY = 1;
			block.UV_SIDE_PY = 1;
			block.UV_SIDE_PX = 1;
			block.UV_SIDE_NX = 1;
			block.UV_SIDE_PZ = 1;
			block.UV_SIDE_NZ = 1;
		}
		if (id == STONE) {

			block.UV_SIDE_NY = 4;
			block.UV_SIDE_PY = 4;
			block.UV_SIDE_PX = 4;
			block.UV_SIDE_NX = 4;
			block.UV_SIDE_PZ = 4;
			block.UV_SIDE_NZ = 4;
		}

		if (id == SAND) {

			block.UV_SIDE_NY = 5;
			block.UV_SIDE_PY = 5;
			block.UV_SIDE_PX = 5;
			block.UV_SIDE_NX = 5;
			block.UV_SIDE_PZ = 5;
			block.UV_SIDE_NZ = 5;
		}
		if (id == WATER) {
			block.UV_SIDE_NY = 6;
			block.UV_SIDE_PY = 6;
			block.UV_SIDE_PX = 6;
			block.UV_SIDE_NX = 6;
			block.UV_SIDE_PZ = 6;
			block.UV_SIDE_NZ = 6;
		}
		if (id == OAK_LOG) {
			block.UV_SIDE_NY = 8;
			block.UV_SIDE_PY = 8;
			block.UV_SIDE_PX = 7;
			block.UV_SIDE_NX = 7;
			block.UV_SIDE_PZ = 7;
			block.UV_SIDE_NZ = 7;
		}
		if (id == OAK_LEAF) {
			block.UV_SIDE_NY = 9;
			block.UV_SIDE_PY = 9;
			block.UV_SIDE_PX = 9;
			block.UV_SIDE_NX = 9;
			block.UV_SIDE_PZ = 9;
			block.UV_SIDE_NZ = 9;
		}
		

	}
	unsigned int get_PY() {
		return block.UV_SIDE_PY;
	}
	unsigned int get_NY() {
		return block.UV_SIDE_NY;
	}
	unsigned int get_PX() {
		return block.UV_SIDE_PX;
	}
	unsigned int get_NX() {
		return block.UV_SIDE_NX;
	}
	unsigned int get_PZ() {
		return block.UV_SIDE_PZ;
	}
	unsigned int get_NZ() {
		return block.UV_SIDE_NZ;
	}
private:
	Block_UVS block;
};



#endif
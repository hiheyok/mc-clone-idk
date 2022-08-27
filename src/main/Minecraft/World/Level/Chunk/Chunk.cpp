#include "Chunk.h"

#include "../../../../utils/MathHelper.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

double TREE_RAND_VAL = 0.5;
double TREE_RAND_VAL_RANGE = .01f;

void Chunk::gen_chunk(FastNoiseLite* noise) {



	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;


	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {

			float BIOME_MAP = (float)(noise->GetNoise((float)x / 600, (float)z / 600, (float)SEED) + 1) / 2;
			float TREE_MAP = (float)((double)(noise->GetNoise((float)x * 10, (float)z * 10, (float)SEED) + 1) / 2) * (BIOME_MAP * 2);
			float MIN_BIOME_MAP = (float)((noise->GetNoise((float)x / 60, (float)z / 60, (float)SEED) + 1)) * 2;
			float TEMP_MAP = (float)(noise->GetNoise((float)x / 1000, (float)z / 1000, (float)SEED) + 1) * 2;
			float HEIGHT_MAP = (float)(((noise->GetNoise((float)x, (float)z, (float)SEED) + 1) * 3) * (((noise->GetNoise((float)x * 5, (float)z * 5, (float)SEED) + 1) * .25) * BIOME_MAP)) * 30;
			int a = (int)pow(((BIOME_MAP * HEIGHT_MAP + 5) * TEMP_MAP - 10) / (2 * MIN_BIOME_MAP), 1.6) + 5;
			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {
				if ((a == y || a + 1 == y || a + 2 == y || a + 3 == y || a + 4 == y) && a > 10) {
					if (TREE_MAP >= (TREE_RAND_VAL - TREE_RAND_VAL_RANGE) && TREE_MAP <= (TREE_RAND_VAL + TREE_RAND_VAL_RANGE)) {
						addblock(x, y, z, OAK_LOG);

					}
				}
				if (a + 5 == y && a > 10) {
					if (TREE_MAP >= (TREE_RAND_VAL - TREE_RAND_VAL_RANGE) && TREE_MAP <= (TREE_RAND_VAL + TREE_RAND_VAL_RANGE)) {
						for (int tx = -2; tx <= 2; tx++) {
							for (int ty = -2; ty <= 2; ty++) {
								for (int tz = -2; tz <= 2; tz++) {
									addblock(x + tx, y + ty, z + tz, OAK_LEAF);
								}
							}
						}

					}
				}
				if (y < 10) {
					addblock(x, y, z, WATER);
				}

				if (a > y) {
					if (y < 12) {
						addblock(x, y, z, SAND);
					}
					else {

						if (a >= y) {
							addblock(x, y, z, GRASS);
						}
						if (a - 1 > y) {
							addblock(x, y, z, DIRT);
						}

						//std::cout << a << "\n";



					}

				}


			}

		}
	}
	//std::string str = std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ".mccvv1";
//	dumpMem(str.c_str());
}

void Chunk::gen_chunkTerrain(FastNoiseLite* noise) {



	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;

	float amplified = 2.0f;
	float nscale = 5.0f;


	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {

			float h = (noise->GetNoise((float)x, (float)z, (float)SEED) + 1) * 0.5f;
			float wideMap = ((noise->GetNoise((float)x / 10, (float)z / 10, (float)SEED) + 1) * 0.5f);

			float h2 = ((noise->GetNoise((float)x, (float)z, (float)SEED))) * 10;

			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {



				float xn = noise->GetNoise((float)x * nscale , (float)y * nscale + h2, (float)z * nscale ) * amplified * 3;

				float HEIGHT_MAP = (xn + y) * h;
				
				if (100 >= HEIGHT_MAP && 17 <= HEIGHT_MAP) {
					if (y < 33) {
						addblock(x, y, z, WATER);
					}
				}
				if (17 >= HEIGHT_MAP && 16 <= HEIGHT_MAP) {
					addblock(x, y, z, GRASS);
				}
				if (16 >= HEIGHT_MAP && -100 <= HEIGHT_MAP) {
					addblock(x, y, z, DIRT);
				}
			}
		}
	}
	/*
	
	local xn = noise(px*pnscale+.1+OffSet[1],py*pnscale+.1,Seed*pnscale)*pampilfied
	local yn = noise(py*pnscale+.1,pz*pnscale+.1,Seed*pnscale+OffSet[2])*pampilfied
	local zn = noise(pz*pnscale+.1+OffSet[2],px*pnscale+.1,Seed*pnscale+OffSet[1])*pampilfied

	if gameRules['CaveMode'] == true then
		local funcFinal = xn+yn+zn
		if funcFinal <=16  and funcFinal>-100 then
			return 1
		elseif funcFinal <=19  and funcFinal>16 then
			return 1
		elseif funcFinal <20 and funcFinal>19 then
			return 2

		end
	else
		local funcFinal = xn+yn+zn+py
		if funcFinal <=16  and funcFinal>-100 then
			return 1
		elseif funcFinal <=19  and funcFinal>16 then
			return 1
		elseif funcFinal <20 and funcFinal>19 then
			return 2

		end
	end*/

}

void Chunk::gen_chunkSphere() {



	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;


	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {
			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {
				if (FindDistance(x, y, z, 0, 100, 0) < 100) {
					addblock(x, y, z, SAND);
				}
			}
		}
	}
}

void Chunk::gen_chunkPerlinCave(FastNoiseLite* noise) {



	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;

	for (int x = cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = cz; z < CHUNK_SIZE + cz; z++)
			for (int y = cy; y < CHUNK_SIZE + cy; y++) {
				if (noise->GetNoise((float)x, (float)y, (float)z) > 0) {
					addblock(x, y, z, OAK_LOG);
				}
			}
	}

}

void Chunk::gen_chunkFlat() {



	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;


	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {
			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {
				if (y == 10) {
					addblock(x, y, z, GRASS);
				}
				if (y < 10) {
					addblock(x, y, z, DIRT);
				}
			}
		}
	}
}

unsigned int extractBlockData(char type, unsigned int data) {
	return NULL;
}

//void Chunk::gen_chunk(FastNoiseLite* noise) {
//
//	int cx = pos.x * 16;
//	int cz = pos.z * 16;
//	int cy = pos.y * 16;
//
//
//	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
//		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {
//
//			
//			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {
//				float HEIGHT = ((noise->GetNoise((float)x * 3, (float)y * 3, (float)z * 3) + 1) / 2) * 32 + y;
//				if (HEIGHT < 40) {
//					addblock(x,y,z,STONE);
//				}
//
//			}
//
//		}
//	}
//
//}

/*
void Chunk::gen_chunk(FastNoiseLite* noise) {

	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;


	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {

			float BIOME_MAP = (noise->GetNoise((float)x / 600, (float)z / 600, (float)SEED) + 1) / 2;
			float HEIGHT_MAP = (((noise->GetNoise((float)x / 3, (float)z / 3, (float)SEED) + 1) * 3) * (((noise->GetNoise((float)x * 5, (float)z * 5, (float)SEED) + 1) * .25) * BIOME_MAP)) * 30;
			float a = BIOME_MAP * HEIGHT_MAP + 5;
			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {

				if (y < 10) {
					addblock(x, y, z, WATER);
				}

				if (a > y) {
					if (y < 12) {
						addblock(x, y, z, SAND);
					}
					else {
						addblock(x, y, z, GRASS);
					}

				}

			}

		}
	}

}
*/
Block Chunk::checkblock(int x, int y, int z) {

	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		if (x < 0) {

			if (getNeighborAddressNX() != nullptr) {
				return neighbors[3]->checkblock(x + CHUNK_SIZE, y, z);
			}
			else {
				return null_block;
			}

		}

		if (x >= CHUNK_SIZE) {
			if (getNeighborAddressPX() != nullptr) {
				return neighbors[2]->checkblock(x - CHUNK_SIZE, y, z);

			}
			else {
				return null_block;
			}

		}

		if (y < 0) {

			if (getNeighborAddressNY() != nullptr) {
				return neighbors[1]->checkblock(x, y + CHUNK_SIZE, z);
			}
			else {
				return null_block;
			}

		}

		if (y >= CHUNK_SIZE) {
			if (getNeighborAddressPY() != nullptr) {
				return neighbors[0]->checkblock(x, y - CHUNK_SIZE, z);

			}
			else {
				return null_block;
			}

		}

		if (z < 0) {

			if (getNeighborAddressNZ() != nullptr) {
				return neighbors[5]->checkblock(x, y, z + CHUNK_SIZE);
			}
			else {
				return null_block;
			}

		}

		if (z >= CHUNK_SIZE) {
			if (getNeighborAddressPZ() != nullptr) {
				return neighbors[4]->checkblock(x, y, z - CHUNK_SIZE);

			}
			else {
				return null_block;
			}

		}


	}
	else {
		return data[(x * CHUNK_SIZE * CHUNK_SIZE) + (z * CHUNK_SIZE) + y];
	}
	return null_block;
}



void Chunk::addblock(int x, int y, int z, unsigned char type) {

	x = x - (pos.x * CHUNK_SIZE);
	y = y - (pos.y * CHUNK_SIZE);
	z = z - (pos.z * CHUNK_SIZE);

	empty = false;

	if (!(x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0)) {
		Block block;
		block.id = type;
		data[(x * CHUNK_SIZE_2) + (z * CHUNK_SIZE) + y] = block;
	}


}

bool Chunk::checkBlockTransparency(unsigned char type) {
	//Transparent
	if (type == AIR)
		return true;
	if (type == WATER)
		return true;
	//false
	if (type == GRASS)
		return false;
	if (type == DIRT)
		return false;
	if (type == COBBLESTONE)
		return false;
	if (type == STONE)
		return false;
	if (type == SAND)
		return false;
	if (type == OAK_LEAF)
		return true;
	if (type == OAK_LOG)
		return false;
}

/*#include "Chunk.h"

#include "SimplexNoise.h"

Chunk GenerateChunk(int x, int y, int z) {

	Chunk chunk;

	int cx = x * CHUNK_SIZE;
	int cz = z * CHUNK_SIZE;
	int cy = y * CHUNK_SIZE;




	chunk.initChunk();

	chunk.setChunkPosition(x,y,z);

	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {
			float BIOME_MAP = (SimplexNoise::noise((float)x / 600, (float)z / 600, (float)SEED) + 1) / 2;
			float MAP_2 = (SimplexNoise::noise((float)x / 3000.f, (float)z / 3000.f, (float)SEED) + 1) / 4 + 2;
			float MAP_3 = (SimplexNoise::noise((float)x / 10.f, (float)z / 10.f, (float)SEED) + 1) * 3 * BIOME_MAP;

			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {
				float a = ((((SimplexNoise::noise((float)x / (30 * MAP_2), (float)z / (30 * MAP_2)) + 1) / 2) * 32 * BIOME_MAP) + MAP_3)*1;

				if (y < a) {
					if (y <= 4) {
						chunk.addblock(x, y, z, SAND);
					}
					else {
						chunk.addblock(x, y, z, GRASS);
					}

				}

				if (y == 0) {
					chunk.addblock(x, 0, z, STONE);
				}

			}

		}
	}


	std::string sx = std::to_string(x);
	std::string sy = std::to_string(y);
	std::string sz = std::to_string(z);

	std::ofstream file;

	file.open(sx + ", " + sy + ", " + sz + ".pre");
	for (int ax = 0; ax < CHUNK_SIZE; ax++) {
		for (int ay = 0; ay < CHUNK_SIZE; ay++) {
			for (int az = 0; az < CHUNK_SIZE; az++) {
				file << (unsigned char)chunk.checkblock(ax,ay,az);;
			}
		}
	}

	return chunk;

}*/
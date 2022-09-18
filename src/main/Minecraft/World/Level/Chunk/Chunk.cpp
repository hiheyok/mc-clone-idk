#include "Chunk.h"

#include "../../../../utils/MathHelper.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

double TREE_RAND_VAL = 0.5;
double TREE_RAND_VAL_RANGE = .01f;

struct vector2 {
	float x = 0;
	float y = 0;
	float angle = 0;
	float length = 0;
};

vector2 subtractVec2(vector2 a, vector2 b) {
	vector2 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}


float dotproduct(vector2 a, vector2 b) {
	return (a.x * b.x) + (a.y * b.y);
}

float interpolate(float val) {
	return 3 * pow(val, 2) - 2 * pow(val, 3);
}

float getNoise(vector2 pos) {
	//gradient
	vector2 tl, tr, bl, br;
	tl.x = floor(pos.x);
	tl.y = floor(pos.y) + 1.0f;

	tr.x = floor(pos.x) + 1.0f;
	tr.y = floor(pos.y) + 1.0f;

	bl.x = floor(pos.x);
	bl.y = floor(pos.y);

	br.x = floor(pos.x) + 1.0f;
	br.y = floor(pos.y);

	vector2 gtl, gtr, gbl, gbr;
	gtl = tl;
	gtr = tr;
	gbr = br;
	gbl = bl;

	gtl = subtractVec2(pos, gtl);
	gtr = subtractVec2(pos, gtr);
	gbl = subtractVec2(pos, gbl);
	gbr = subtractVec2(pos, gbr);

	float s = dotproduct(tl, gtl);
	float t = dotproduct(tr, gtr);
	float u = dotproduct(br, gbr);
	float v = dotproduct(bl, gbl);

	float a = interpolate(t - s);
	float b = interpolate(v - u);

	return interpolate(a - b);


}

void Chunk::gen_chunk(FastNoiseLite* noise) {

	noise->SetFrequency(0.009f);

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

#include "../../../../Utils/LogUtils.h"

void Chunk::gen_chunkTerrain(FastNoiseLite* noise) {



	int cx = pos.x * CHUNK_SIZE;
	int cz = pos.z * CHUNK_SIZE;
	int cy = pos.y * CHUNK_SIZE;

	float amplified = 2.0f;
	float nscale = 5.0f;


	for (int x = 0 + cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = 0 + cz; z < CHUNK_SIZE + cz; z++) {

			vector2 a;
			a.x = x / 1000.f;
			a.y = z / 1000.f;
			float h = getNoise(a);

			getLogger()->LogDebug("World", "Noise: " + std::to_string(h));

			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {

				
				if (y < h) {
					addblock(x,y,z, STONE);
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
		return data[(x * 256) + (z * 16) + y];
	}
	return null_block;
}

float Chunk::anoise(FastNoiseLite* noise, int x, int y, int z, float frequency, int octaves) {
	float total = 0;

	for (int i = 0; i < octaves; i++) {
		float fx = (float)(x * pow2LookupMut[i]) * frequency;
		float fy = (float)(y * pow2LookupMut[i]) * frequency;
		float fz = (float)(z * pow2LookupMut[i]) * frequency;
		float fn = noise->GetNoise(fx, fy, fz);
		total = total + fn * pow2LookupDiv[i];
	}

	return total * normalLookupMut[octaves];
}

float Chunk::anoise(FastNoiseLite* noise, int x, int z, float frequency, int octaves) {
	float total = 0;

	for (int i = 0; i < octaves; i++) {
		float fx = (float)(x * pow2LookupMut[i]) * frequency;
		float fz = (float)(z * pow2LookupMut[i]) * frequency;
		float fn = noise->GetNoise(fx, fz);
		total = total + fn * pow2LookupDiv[i];
	}

	return total * normalLookupMut[octaves];
}

void Chunk::gen_chunkMinecraft(FastNoiseLite* noise) {
	noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

	noise->SetSeed(254);

	int cx = pos.x * 16;
	int cz = pos.z * 16;
	int cy = pos.y * 16;
	
	float height = 32;
	float dheight = 0.5;
	float mheight = 256;

	noise->SetFrequency(.00125);

	for (int x = cx; x < CHUNK_SIZE + cx; x++) {
		for (int z = cz; z < CHUNK_SIZE + cz; z++) {
			float continental = anoise(noise, x, z, 1, 5);
			float cny = continentialNoiseHeight(continental);
			for (int y = 0 + cy; y < CHUNK_SIZE + cy; y++) {

				float density = anoise(noise, x, y, z, 8, 3);

				density = density + 1;
				density = density * 0.5f;

				float y1 = y - cny;

				if (0.5f > density * (y1 * dheight)) {
					addblock(x, y, z, STONE);
				}
			}
		}
	}
}



int Chunk::continentialNoiseHeight(float n) {
	n = n + 1;
	n = n * 0.5f;

	glm::vec2 s1;
	glm::vec2 s2;

	for (int i = 0; i < continentialMapSample.size(); i++) {
		if (continentialMapSample[i].x < n && continentialMapSample[i + 1].x > n) {
			s1 = continentialMapSample[i];
			s2 = continentialMapSample[i + 1];
			break;
		}
	}

	float m0 = (n - (s2.x + s1.x) * 0.5f) / (s1.x - s2.x);
	float m1 = (1 - cos((m0 - 0.5) * 3.141592f)) * 0.5f;


	float f = s1.y + (s2.y - s1.y) * m1;
	return f;

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
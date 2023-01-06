#include "TerrainGen.h"
#include "../../../../Utils/Math/ChunkID/ChunkID.h"
#include "../../../../Utils/Math/Vector/vec3.h"
#include "../../../../Utils/FastNoiseLite.h"

void GenerateTerrain(Chunk& chunk) {
	ivec3 Pos = ChunkIDToPOS(chunk.ChunkID) * 16;

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetSeed(345.5f);

	double TREE_RAND_VAL = 0.5;
	double TREE_RAND_VAL_RANGE = .01f;

	for (int x = 0 + Pos.x; x < CHUNK_SIZE + Pos.x; x++) {
		for (int z = 0 + Pos.z; z < CHUNK_SIZE + Pos.z; z++) {

			float BIOME_MAP = (float)(noise.GetNoise((float)x / 600, (float)z / 600) + 1) / 2;
			float TREE_MAP = (float)((double)(noise.GetNoise((float)x * 10, (float)z * 10) + 1) / 2) * (BIOME_MAP * 2);
			float MIN_BIOME_MAP = (float)((noise.GetNoise((float)x / 60, (float)z / 60) + 1)) * 2;
			float TEMP_MAP = (float)(noise.GetNoise((float)x / 1000, (float)z / 1000) + 1) * 2;
			float HEIGHT_MAP = (float)(((noise.GetNoise((float)x, (float)z) + 1) * 3) * (((noise.GetNoise((float)x * 5, (float)z * 5) + 1) * .25) * BIOME_MAP)) * 30;
			int a = (int)pow(((BIOME_MAP * HEIGHT_MAP + 5) * TEMP_MAP - 10) / (2 * MIN_BIOME_MAP), 1.6) + 5;
			for (int y = 0 + Pos.y; y < CHUNK_SIZE + Pos.y; y++) {
				if ((a == y || a + 1 == y || a + 2 == y || a + 3 == y || a + 4 == y) && a > 10) {
					if (TREE_MAP >= (TREE_RAND_VAL - TREE_RAND_VAL_RANGE) && TREE_MAP <= (TREE_RAND_VAL + TREE_RAND_VAL_RANGE)) {
						chunk.addBlock(x, y, z, OAK_LOG);

					}
				}
				if (a + 5 == y && a > 10) {
					if (TREE_MAP >= (TREE_RAND_VAL - TREE_RAND_VAL_RANGE) && TREE_MAP <= (TREE_RAND_VAL + TREE_RAND_VAL_RANGE)) {
						for (int tx = -2; tx <= 2; tx++) {
							for (int ty = -2; ty <= 2; ty++) {
								for (int tz = -2; tz <= 2; tz++) {
									chunk.addBlock(x + tx, y + ty, z + tz, OAK_LEAF);
								}
							}
						}

					}
				}
				if (y < 10) {
					chunk.addBlock(x, y, z, WATER);
				}

				if (a > y) {
					if (y < 12) {
						chunk.addBlock(x, y, z, SAND);
					}
					else {

						if (a >= y) {
							chunk.addBlock(x, y, z, GRASS_BLOCK);
						}
						if (a - 1 > y) {
							chunk.addBlock(x, y, z, DIRT);
						}

					}

				}


			}

		}
	}

}
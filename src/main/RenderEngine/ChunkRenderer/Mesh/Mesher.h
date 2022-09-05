#ifndef MESH_H_2
#define MESH_H_2




#include "../../../Minecraft/World/Level/Chunk/Chunk.h"
#include "../../../utils/MathHelper.h"

#include <unordered_map>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>

#include "../../../utils/FastNoiseLite.h"
#include "../../../utils/MathHelper.h"
#include "../../../utils/Clock.h"
#include "../../../utils/LogUtils.h"

#include <vector>
#include <algorithm>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <string>



/*
BLOCK DATA (32 BIT):
BIT_OFFSET		TYPE			SIZE
0				ID				8
8				Transparency	1
8				LIGHT			4


*/

/* 
VDATA (32 BIT):
BIT_OFFSET		TYPE			SIZE
0				ID				8
8				L0				4
12				L1				4
16				L2				4
20				L3				4
*/


struct ChunkVerticesData {
	std::vector<unsigned int> TransparentVertices;
	std::vector<unsigned int> SolidVertices;
	int x = 0;
	int y = 0;
	int z = 0;

};

struct BlockVerticesData {
	unsigned int Vdata[6]{};
	Block data;
};

class SpecialChunkMesh {
public:
	void addUninitBlock(int x, int y, int z, char B_ID);
	BlockVerticesData extract(int x, int y, int z);
	bool compare(int x, int y, int z, int x1, int y1, int z1, char type);
	unsigned int extractVData(char type, unsigned int data);
	BlockVerticesData data[CHUNK_SIZE_3]{};
	void add(int x, int y, int z, char B_ID, unsigned int L0_, unsigned int L1_, unsigned int L2_, unsigned int L3_, char side);
};

class ChunkMesh {
public:

	Chunk* chunk = nullptr;

	void SmartGreedyMeshing();

	std::vector<unsigned int> vertices;
	std::vector<unsigned int> transparentVertices;

	void delete_();

	void dumpData(const char* filename) {
		std::ofstream file;
		file.open(filename);
		for (int index = 0; index < vertices.size(); index++) {
			file << vertices[index];
		}
		file.close();
	}

	bool init = false;

	glm::ivec3 pos = glm::ivec3(0, 0, 0);


private:

	void SaddSidenx(int x, int y, int z, glm::ivec3 local_pos, char B_ID);
	void SaddSidepx(int x, int y, int z, glm::ivec3 local_pos, char B_ID);

	void SaddSideny(int x, int y, int z, glm::ivec3 local_pos, char B_ID);
	void SaddSidepy(int x, int y, int z, glm::ivec3 local_pos, char B_ID);

	void SaddSidenz(int x, int y, int z, glm::ivec3 local_pos, char B_ID);
	void SaddSidepz(int x, int y, int z, glm::ivec3 local_pos, char B_ID);

	void SGaddSidenx(glm::ivec3 p0, glm::ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1);
	void SGaddSidepx(glm::ivec3 p0, glm::ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1);

	void SGaddSideny(glm::ivec3 p0, glm::ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1);
	void SGaddSidepy(glm::ivec3 p0, glm::ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1);

	void SGaddSidenz(glm::ivec3 p0, glm::ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1);
	void SGaddSidepz(glm::ivec3 p0, glm::ivec3 p1, bool transparency, int sy, int sx, int a, int b, int a1, int b1);

	unsigned int tnx_ = 0;
	unsigned int tpx_ = 0;
	unsigned int tny_ = 0;
	unsigned int tpy_ = 0;
	unsigned int tnz_ = 0;
	unsigned int tpz_ = 0;

	char BLOCK_ID = AIR;

	bool checkUseX(int x, int y, int z);
	bool checkUseY(int x, int y, int z);
	bool checkUseZ(int x, int y, int z);
	bool checkUseYN(int x, int y, int z);
	bool checkUseXN(int x, int y, int z);
	bool checkUseZN(int x, int y, int z);

	bool useX[(16 + 1) * (16 + 1) * (16 + 1)]{};
	bool useY[(16 + 1) * (16 + 1) * (16 + 1)]{};
	bool useZ[(16 + 1) * (16 + 1) * (16 + 1)]{};

	bool useXN[(16 + 1) * (16 + 1) * (16 + 1)]{};
	bool useYN[(16 + 1) * (16 + 1) * (16 + 1)]{};
	bool useZN[(16 + 1) * (16 + 1) * (16 + 1)]{};

	SpecialChunkMesh* SMesh = nullptr;
	
};



#endif
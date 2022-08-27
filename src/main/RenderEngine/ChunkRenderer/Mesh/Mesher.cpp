#include <glm/vec3.hpp>
#include <list>
#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iterator>
#include <chrono>

#include "../../../Minecraft/World/Level/Chunk/Chunk.h"
#include "TexMap.h"
#include "Mesher.h"

#define px 0x00
#define py 0x01
#define pz 0x02
#define nx 0x03
#define ny 0x04
#define nz 0x05

#define ID 0x00
#define L0 0x01
#define L1 0x02
#define L2 0x03
#define L3 0x04

const int USE_SIZE = CHUNK_SIZE + 1;

const int USE_SIZE_2 = USE_SIZE * USE_SIZE;

using namespace std;

using namespace glm;

bool enableBlockShading_ = true;

int nx1_ = 0;
int px1_ = 0;
int ny1_ = 0;
int py1_ = 0;
int nz1_ = 0;
int pz1_ = 0;

int lightCvalue_ = 13;
int lightMvalue_ = 15;

int xDataBitOffset = 0;
int yDataBitOffset = 5;
int zDataBitOffset = 10;
int blockShadingBitOffset = 15;
int textureBitOffset = 20;


bool ChunkMesh::checkUseX(int x, int y, int z) {
//	if (x >= USE_SIZE || y >= USE_SIZE || z >= USE_SIZE || x < 0 || y < 0 || z < 0)
//		return false;
	return useX[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseY(int x, int y, int z) {
//	if (x >= USE_SIZE || y >= USE_SIZE || z >= USE_SIZE || x < 0 || y < 0 || z < 0)
//		return false;
	return useY[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseZ(int x, int y, int z) {
//	if (x >= USE_SIZE || y >= USE_SIZE || z >= USE_SIZE || x < 0 || y < 0 || z < 0)
//		return false;
	return useZ[x * USE_SIZE_2 + y * USE_SIZE + z];
}


bool ChunkMesh::checkUseXN(int x, int y, int z) {
//	if (x >= USE_SIZE || y >= USE_SIZE || z >= USE_SIZE || x < 0 || y < 0 || z < 0)
//		return false;
	return useXN[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseYN(int x, int y, int z) {
//	if (x >= USE_SIZE || y >= USE_SIZE || z >= USE_SIZE || x < 0 || y < 0 || z < 0)
//		return false;
	return useYN[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseZN(int x, int y, int z) {
//	if (x >= USE_SIZE || y >= USE_SIZE || z >= USE_SIZE || x < 0 || y < 0 || z < 0)
//		return false;
	return useZN[x * USE_SIZE_2 + y * USE_SIZE + z];
}

void ChunkMesh::delete_() {
	vertices.clear();
	transparentVertices.clear();
}

void ChunkMesh::SmartGreedyMeshing() {

	if (chunk->isEmpty())
		return;

	auto t0 = std::chrono::high_resolution_clock::now();

	SMesh = new SpecialChunkMesh;

	//Gen Data

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				if (!chunk->checkblock(x, y, z).transparent()) {

					if (chunk->checkblock(x - 1, y, z).transparent()) {
						SaddSidenx(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x + 1, y, z).transparent()) {
						SaddSidepx(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y - 1, z).transparent()) {
						SaddSideny(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y + 1, z).transparent()) {
						SaddSidepy(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y, z - 1).transparent()) {
						SaddSidenz(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y, z + 1).transparent()) {
						SaddSidepz(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}

				}

				if (chunk->checkblock(x, y, z).id == OAK_LEAF) {

					if (chunk->checkblock(x - 1, y, z).transparent()) {
						SaddSidenx(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x + 1, y, z).transparent()) {
						SaddSidepx(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y - 1, z).transparent()) {
						SaddSideny(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y + 1, z).transparent()) {
						SaddSidepy(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y, z - 1).transparent()) {
						SaddSidenz(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}
					if (chunk->checkblock(x, y, z + 1).transparent()) {
						SaddSidepz(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}

				}

				if (chunk->checkblock(x, y, z).id == WATER) {
					if (chunk->checkblock(x, y + 1, z).id == AIR) {
						SaddSidepy(x, y, z, ivec3(x, y, z), chunk->checkblock(x, y, z).id);
					}


				}

			}
		}
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	//Process Data and Gen Mesh

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				int x0 = x;
				int y0 = y;
				int z0 = z;
				int x1 = x;
				int y1 = y;
				int z1 = z;

				unsigned int VDATA[6]{};

				VDATA[px] = SMesh->extract(x, y, z).Vdata[px];
				VDATA[nx] = SMesh->extract(x, y, z).Vdata[nx];
				VDATA[py] = SMesh->extract(x, y, z).Vdata[py];
				VDATA[ny] = SMesh->extract(x, y, z).Vdata[ny];
				VDATA[pz] = SMesh->extract(x, y, z).Vdata[pz];
				VDATA[nz] = SMesh->extract(x, y, z).Vdata[nz];

				bool transparent = SMesh->extract(x, y, z).data.transparent();

				block_uv BLOCK;
				BLOCK.setBlockUVS(SMesh->extract(x, y, z).data.id);

				//PY
				if ((SMesh->extractVData(ID, VDATA[py]) != AIR)) {
					tpy_ = BLOCK.get_PY();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (x1 < CHUNK_SIZE) {
						if ((!checkUseY(x1, y, z)) && (VDATA[py] == SMesh->extract(x1, y, z).Vdata[py]) && (SMesh->extract(x1, y + 1, z).data.transparent())) {
							useY[x1 * USE_SIZE_2 + (y)*USE_SIZE + z] = true;
							x1++;
						}
						else { break; }
					}
					while (z1 < CHUNK_SIZE) {
						z1++;
						bool break_ = false;
						int dx = x0;
						while (dx < x1) {
							if ((!checkUseY(dx, y, z1)) && (VDATA[py] == SMesh->extract(dx, y, z1).Vdata[py]) && (SMesh->extract(dx, y + 1, z1).data.transparent())) {
								// if ((!checkUseY(dx, y, z1)) && (SMesh->compare(x,y,z,dx,y+1,z1,py))) {
								dx++;
							}
							else {
								break_ = true;
								break;
							}
						}
						if (break_) { break; }
						for (int dx1 = x0; dx1 < x1; dx1++) {
							useY[dx1 * USE_SIZE_2 + (y)*USE_SIZE + z1] = true;
						}
					}
					unsigned int quad = SMesh->extract(x, y, z).Vdata[py];
					if (!((x0 - x1) == 0 || (z0 - z1) == 0))
						SGaddSidepy(ivec3(x0, y0, z0), ivec3(x1, y1, z1), transparent, x1 - x, z1 - z, SMesh->extractVData(L0, quad), SMesh->extractVData(L1, quad), SMesh->extractVData(L2, quad), SMesh->extractVData(L3, quad));
				}
				//PX
				if ((SMesh->extractVData(ID, VDATA[px]) != AIR)) {
					tpx_ = BLOCK.get_PX();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseX(x, y1, z)) && (VDATA[px] == SMesh->extract(x, y1, z).Vdata[px]) && (chunk->checkblock(x + 1, y1, z).transparent())) {
							useX[x * USE_SIZE_2 + (y1)*USE_SIZE + z] = true;
							y1++;
						}
						else { break; }
					}
					while (z1 < CHUNK_SIZE) {
						z1++;
						bool break_ = false;
						int dy = y0;
						while (dy < y1) {
							if ((!checkUseX(x, dy, z1)) && (VDATA[px] == SMesh->extract(x, dy, z1).Vdata[px]) && (chunk->checkblock(x + 1, dy, z1).transparent())) {
								dy++;
							}
							else {
								break_ = true;
								break;
							}
						}
						if (break_) { break; }
						for (int dy1 = y0; dy1 < y1; dy1++) {
							useX[x * USE_SIZE_2 + (dy1)*USE_SIZE + z1] = true;
						}
					}
					unsigned int quad = SMesh->extract(x, y, z).Vdata[px];
					if (!((y0 - y1) == 0 || (z0 - z1) == 0))
						SGaddSidepx(ivec3(x0, y0, z0), ivec3(x1, y1, z1), transparent, y1 - y, z1 - z, SMesh->extractVData(L3, quad), SMesh->extractVData(L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L2, quad));
				}
				//PZ
				if ((SMesh->extractVData(ID, VDATA[pz]) != AIR)) {
					tpz_ = BLOCK.get_PZ();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseZ(x, y1, z)) && (VDATA[pz] == SMesh->extract(x, y1, z).Vdata[pz]) && (chunk->checkblock(x, y1, z + 1).transparent())) {
							useZ[x * USE_SIZE_2 + (y1)*USE_SIZE + z] = true;
							y1++;
						}
						else {
							break;
						}
					}
					while (x1 < CHUNK_SIZE) {
						x1++;
						bool break_ = false;
						int dy = y0;
						while (dy < y1) {
							if ((!checkUseZ(x1, dy, z)) && (VDATA[pz] == SMesh->extract(x1, dy, z).Vdata[pz]) && (chunk->checkblock(x1, dy, z + 1).transparent())) {
								dy++;
							}
							else {
								break_ = true;
								break;
							}
						}
						if (break_)
							break;
						for (int dy1 = y0; dy1 < y1; dy1++) {
							useZ[x1 * USE_SIZE_2 + (dy1)*USE_SIZE + z] = true;
						}
					}
					unsigned int quad = SMesh->extract(x, y, z).Vdata[pz];
					if (!((y0 - y1) == 0 || (x0 - x1) == 0))
						SGaddSidepz(ivec3(x0, y0, z0), ivec3(x1, y1, z1), transparent, x1 - x, y1 - y, SMesh->extractVData(L2, quad), SMesh->extractVData(L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L3, quad));
				}


				//NEGATIVES
				//NY
				if ((SMesh->extractVData(ID, VDATA[ny]) != AIR)) {
					tny_ = BLOCK.get_NY();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (x1 < CHUNK_SIZE) {
						if ((!checkUseYN(x1, y, z)) && (VDATA[ny] == SMesh->extract(x1, y, z).Vdata[ny]) && (SMesh->extract(x1, y - 1, z).data.transparent())) {
							useYN[x1 * USE_SIZE_2 + (y)*USE_SIZE + z] = true;
							x1++;
						}
						else { break; }
					}
					while (z1 < CHUNK_SIZE) {
						z1++;
						bool break_ = false;
						int dx = x0;
						while (dx < x1) {
							if ((!checkUseYN(dx, y, z1)) && (VDATA[ny] == SMesh->extract(dx, y, z1).Vdata[ny]) && (SMesh->extract(dx, y - 1, z1).data.transparent())) {
								dx++;
							}
							else {
								break_ = true;
								break;
							}
						}
						if (break_) { break; }
						for (int dx1 = x0; dx1 < x1; dx1++) {
							useYN[dx1 * USE_SIZE_2 + (y)*USE_SIZE + z1] = true;
						}
					}
					unsigned int quad = SMesh->extract(x, y, z).Vdata[ny];
					if (!((x0 - x1) == 0 || (z0 - z1) == 0))
						SGaddSideny(ivec3(x0, y0, z0), ivec3(x1, y1, z1), transparent, x1 - x, z1 - z, SMesh->extractVData(L0, quad), SMesh->extractVData(L1, quad), SMesh->extractVData(L2, quad), SMesh->extractVData(L3, quad));
				}
				//NX
				if ((SMesh->extractVData(ID, VDATA[nx]) != AIR)) {
					tnx_ = BLOCK.get_NX();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseXN(x, y1, z)) && (VDATA[nx] == SMesh->extract(x, y1, z).Vdata[nx]) && (chunk->checkblock(x - 1, y1, z).transparent())) {
							useXN[x * USE_SIZE_2 + (y1)*USE_SIZE + z] = true;
							y1++;
						}
						else { break; }
					}
					while (z1 < CHUNK_SIZE) {
						z1++;
						bool break_ = false;
						int dy = y0;
						while (dy < y1) {
							if ((!checkUseXN(x, dy, z1)) && (VDATA[nx] == SMesh->extract(x, dy, z1).Vdata[nx]) && (chunk->checkblock(x - 1, dy, z1).transparent())) {
								dy++;
							}
							else {
								break_ = true;
								break;
							}
						}
						if (break_) { break; }
						for (int dy1 = y0; dy1 < y1; dy1++) {
							useXN[x * USE_SIZE_2 + (dy1)*USE_SIZE + z1] = true;
						}
					}
					unsigned int quad = SMesh->extract(x, y, z).Vdata[nx];
					if (!((y0 - y1) == 0 || (z0 - z1) == 0))
						SGaddSidenx(ivec3(x0, y0, z0), ivec3(x1, y1, z1), transparent, y1 - y, z1 - z, SMesh->extractVData(L3, quad), SMesh->extractVData(L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L2, quad));
				}
				//PZ
				if ((SMesh->extractVData(ID, VDATA[nz]) != AIR)) {
					tnz_ = BLOCK.get_NZ();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseZN(x, y1, z)) && (VDATA[nz] == SMesh->extract(x, y1, z).Vdata[nz]) && (chunk->checkblock(x, y1, z - 1).transparent())) {
							useZN[x * USE_SIZE_2 + (y1)*USE_SIZE + z] = true;
							y1++;
						}
						else {
							break;
						}
					}
					while (x1 < CHUNK_SIZE) {
						x1++;
						bool break_ = false;
						int dy = y0;
						while (dy < y1) {
							if ((!checkUseZN(x1, dy, z)) && (VDATA[nz] == SMesh->extract(x1, dy, z).Vdata[nz]) && (chunk->checkblock(x1, dy, z - 1).transparent())) {
								dy++;
							}
							else {
								break_ = true;
								break;
							}
						}
						if (break_)
							break;
						for (int dy1 = y0; dy1 < y1; dy1++) {
							useZN[x1 * USE_SIZE_2 + (dy1)*USE_SIZE + z] = true;
						}
					}
					unsigned int quad = SMesh->extract(x, y, z).Vdata[nz];
					if (!((y0 - y1) == 0 || (x0 - x1) == 0))
						SGaddSidenz(ivec3(x0, y0, z0), ivec3(x1, y1, z1), transparent, y1 - y , x1 - x, SMesh->extractVData(L2, quad), SMesh->extractVData(L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L3, quad));
				}

			}
		}

	}
	for (int i = 0; i < USE_SIZE * USE_SIZE * USE_SIZE; i++) {
		useX[i] = false;
		useY[i] = false;
		useZ[i] = false;
		useXN[i] = false;
		useYN[i] = false;
		useZN[i] = false;
	}

	delete SMesh;

	double time0 = (double)(std::chrono::high_resolution_clock::now() - t0).count() / (double)1000000;
	double time1 = (double)(t1 - t0).count() / (double)1000000;
	double time2 = time0 - time1;

	//std::cout << time0 << " " << time1 << " " << time2 << " " << vertices.size() + transparentVertices.size() << "\n";
}

BlockVerticesData SpecialChunkMesh::extract(int x, int y, int z) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {
		BlockVerticesData data;
		data.data.id = AIR;
		return data;
	}
	else {
		return data[(x * CHUNK_SIZE * CHUNK_SIZE) + (z * CHUNK_SIZE) + y];
	}
}

void SpecialChunkMesh::add(int x, int y, int z, char B_ID, unsigned int L0_, unsigned int L1_, unsigned int L2_, unsigned int L3_, char side) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {

	}
	else {
		data[(x * CHUNK_SIZE * CHUNK_SIZE) + (z * CHUNK_SIZE) + y].Vdata[side] = 0 | (B_ID) | (L0_ << 8) | (L1_ << 12) | (L2_ << 16) | (L3_ << 20);
		data[(x * CHUNK_SIZE * CHUNK_SIZE) + (z * CHUNK_SIZE) + y].data.id = B_ID;
	}

}
void SpecialChunkMesh::addUninitBlock(int x, int y, int z, char B_ID) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0) {

	}
	else {
		data[x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y].data.id = B_ID;
	}

}
bool SpecialChunkMesh::compare(int x, int y, int z, int x1, int y1, int z1, char type) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE || x < 0 || y < 0 || z < 0 || x1 >= CHUNK_SIZE || y1 >= CHUNK_SIZE || z1 >= CHUNK_SIZE || x1 < 0 || y1 < 0 || z1 < 0)
		return false;
	unsigned int q0 = data[x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y].Vdata[type];
	unsigned int q1 = data[x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y].Vdata[type];
	if (
		(extractVData(ID, q0) == extractVData(ID, q1))
		&& (extractVData(L0, q0) == extractVData(L0, q1))
		&& (extractVData(L1, q0) == extractVData(L1, q1))
		&& (extractVData(L2, q0) == extractVData(L2, q1))
		&& (extractVData(L3, q0) == extractVData(L3, q1))
		)
		return true;
	return false;
}
unsigned int SpecialChunkMesh::extractVData(char type, unsigned int data) {
	if (type == ID)
		return (((1u << 8) - 1u) & (data >> 0));
	if (type == L0)
		return (((1u << 4) - 1u) & (data >> 8));
	if (type == L1)
		return (((1u << 4) - 1u) & (data >> 12));
	if (type == L2)
		return (((1u << 4) - 1u) & (data >> 16));
	if (type == L3)
		return (((1u << 4) - 1u) & (data >> 20));
	return 0;
}


void ChunkMesh::SGaddSidepy(ivec3 p0, ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1) {
	if (transparency) {
		transparentVertices.push_back(0u | (p0.x) | ((1 + p0.y) << yDataBitOffset) | (p1.z << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sy << 10) | (tpy_ << textureBitOffset));
		transparentVertices.push_back(0u | (p0.x) | ((1 + p0.y) << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tpy_ << textureBitOffset));
		transparentVertices.push_back(0u | (p1.x) | ((1 + p0.y) << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tpy_ << textureBitOffset));
		transparentVertices.push_back(0u | (p0.x) | ((1 + p0.y) << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tpy_ << textureBitOffset));
		transparentVertices.push_back(0u | (p1.x) | ((1 + p0.y) << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (0 << 10) | (tpy_ << textureBitOffset));
		transparentVertices.push_back(0u | (p1.x) | ((1 + p0.y) << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tpy_ << textureBitOffset));
	}
	else {
		
		vertices.push_back(0u | (p0.x) | ((1 + p0.y) << yDataBitOffset) | (p1.z << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sy << 10) | (tpy_ << textureBitOffset));
		vertices.push_back(0u | (p0.x) | ((1 + p0.y) << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tpy_ << textureBitOffset));
		vertices.push_back(0u | (p1.x) | ((1 + p0.y) << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tpy_ << textureBitOffset));
		vertices.push_back(0u | (p0.x) | ((1 + p0.y) << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tpy_ << textureBitOffset));
		vertices.push_back(0u | (p1.x) | ((1 + p0.y) << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (0 << 10) | (tpy_ << textureBitOffset));
		vertices.push_back(0u | (p1.x) | ((1 + p0.y) << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tpy_ << textureBitOffset));
	}
}
void ChunkMesh::SGaddSideny(ivec3 p0, ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1) {
	if (transparency) {
		transparentVertices.push_back(0u | (p0.x) | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tny_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | (p0.x) | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sy << 10) | (tny_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | (p1.x) | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tny_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | (p1.x) | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (0 << 10) | (tny_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | (p0.x) | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tny_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | (p1.x) | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tny_ << textureBitOffset) | (1 << 31));
	}
	else {
		vertices.push_back(0u | (p0.x) | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tny_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | (p0.x) | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sy << 10) | (tny_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | (p1.x) | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tny_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | (p1.x) | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (0 << 10) | (tny_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | (p0.x) | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tny_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | (p1.x) | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tny_ << textureBitOffset) | (1 << 31));
	}

}
void ChunkMesh::SGaddSidepx(ivec3 p0, ivec3 p1, bool transparency, int sy, int sx, int a, int b, int a1, int b1) {
	if (transparency) {
		transparentVertices.push_back(0u | p0.x + 1 | (p1.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (0 << 10) | (tpx_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x + 1 | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tpx_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x + 1 | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tpx_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x + 1 | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tpx_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x + 1 | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sy << 10) | (tpx_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x + 1 | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tpx_ << textureBitOffset));
	}
	else {
		vertices.push_back(0u | p0.x + 1 | (p1.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (0 << 10) | (tpx_ << textureBitOffset));
		vertices.push_back(0u | p0.x + 1 | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tpx_ << textureBitOffset));
		vertices.push_back(0u | p0.x + 1 | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tpx_ << textureBitOffset));
		vertices.push_back(0u | p0.x + 1 | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tpx_ << textureBitOffset));
		vertices.push_back(0u | p0.x + 1 | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sy << 10) | (tpx_ << textureBitOffset));
		vertices.push_back(0u | p0.x + 1 | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tpx_ << textureBitOffset));
	}

}
void ChunkMesh::SGaddSidenx(ivec3 p0, ivec3 p1, bool transparency, int sy, int sx, int a, int b, int a1, int b1) {
	if (transparency) {
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (0 << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sy << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sx << 0) | (sy << 10) | (tnx_ << textureBitOffset) | (1 << 31));
	}
	else {
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (0 << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sy << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tnx_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p1.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sx << 0) | (sy << 10) | (tnx_ << textureBitOffset) | (1 << 31));
	}

}
void ChunkMesh::SGaddSidepz(ivec3 p0, ivec3 p1, bool transparency, int sy, int sx, int a, int b, int a1, int b1) {
	if (transparency) {
		transparentVertices.push_back(0u | p1.x | (p1.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tpz_ << textureBitOffset));
		transparentVertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sx << 10) | (tpz_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sy << 0) | (0 << 10) | (tpz_ << textureBitOffset));
		transparentVertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sx << 10) | (tpz_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sy << 0) | (sx << 10) | (tpz_ << textureBitOffset));
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sy << 0) | (0 << 10) | (tpz_ << textureBitOffset));
	}
	else {
		vertices.push_back(0u | p1.x | (p1.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tpz_ << textureBitOffset));
		vertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sx << 10) | (tpz_ << textureBitOffset));
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sy << 0) | (0 << 10) | (tpz_ << textureBitOffset));
		vertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sx << 10) | (tpz_ << textureBitOffset));
		vertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sy << 0) | (sx << 10) | (tpz_ << textureBitOffset));
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | ((p0.z + 1) << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sy << 0) | (0 << 10) | (tpz_ << textureBitOffset));
	}

}
void ChunkMesh::SGaddSidenz(ivec3 p0, ivec3 p1, bool transparency, int sx, int sy, int a, int b, int a1, int b1) {
	if (transparency) {
		transparentVertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sx << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p1.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (0 << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sy << 0) | (0 << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sy << 0) | (sx << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		transparentVertices.push_back(0u | (0 << 0) | (sx << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		transparentVertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		transparentVertices.push_back(0u | (sy << 0) | (0 << 10) | (tnz_ << textureBitOffset) | (1 << 31));

	}
	else {
		vertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sx << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p1.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (0 << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sy << 0) | (0 << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (a1 << blockShadingBitOffset));
		vertices.push_back(0u | (sy << 0) | (sx << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p1.x | (p0.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b << blockShadingBitOffset));
		vertices.push_back(0u | (0 << 0) | (sx << 10) | (tnz_ << textureBitOffset) | (1 << 31));
		vertices.push_back(0u | p0.x | (p1.y << yDataBitOffset) | (p0.z << zDataBitOffset) | (b1 << blockShadingBitOffset));
		vertices.push_back(0u | (sy << 0) | (0 << 10) | (tnz_ << textureBitOffset) | (1 << 31));
	}

}

void ChunkMesh::SaddSideny(int x, int y, int z, ivec3 local_pos, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2]).transparent()) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2]).transparent()) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0], local_pos[1] - 1, local_pos[2] + 1).transparent()) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0], local_pos[1] - 1, local_pos[2] - 1).transparent()) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2] + 1).transparent())
		a1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2] - 1).transparent())
		a = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2] - 1).transparent())
		b1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2] + 1).transparent())
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, ny);
}
void ChunkMesh::SaddSidepy(int x, int y, int z, ivec3 local_pos, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2]).transparent()) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2]).transparent()) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0], local_pos[1] + 1, local_pos[2] + 1).transparent()) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0], local_pos[1] + 1, local_pos[2] - 1).transparent()) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2] + 1).transparent())
		a1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2] - 1).transparent())
		a = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2] - 1).transparent())
		b1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2] + 1).transparent())
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, py);
}
void ChunkMesh::SaddSidenx(int x, int y, int z, ivec3 local_pos, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2]).transparent()) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2]).transparent()) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1], local_pos[2] + 1).transparent()) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1], local_pos[2] - 1).transparent()) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2] + 1).transparent())
		b1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2] - 1).transparent())
		a1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2] - 1).transparent())
		a = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2] + 1).transparent())
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, nx);
}
void ChunkMesh::SaddSidepx(int x, int y, int z, ivec3 local_pos, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2]).transparent()) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2]).transparent()) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1], local_pos[2] + 1).transparent()) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1], local_pos[2] - 1).transparent()) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2] + 1).transparent())
		b1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2] - 1).transparent())
		a1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2] - 1).transparent())
		a = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2] + 1).transparent())
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, px);
}
void ChunkMesh::SaddSidenz(int x, int y, int z, ivec3 local_pos, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!chunk->checkblock(local_pos[0], local_pos[1] - 1, local_pos[2] - 1).transparent()) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0], local_pos[1] + 1, local_pos[2] - 1).transparent()) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1], local_pos[2] - 1).transparent()) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1], local_pos[2] - 1).transparent()) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2] - 1).transparent())
		b1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2] - 1).transparent())
		a1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2] - 1).transparent())
		a = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2] - 1).transparent())
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, nz);
}
void ChunkMesh::SaddSidepz(int x, int y, int z, ivec3 local_pos, char B_ID) {
	char a = lightMvalue_;
	char b = lightMvalue_;
	char a1 = lightMvalue_;
	char b1 = lightMvalue_;
	if (!chunk->checkblock(local_pos[0], local_pos[1] - 1, local_pos[2] + 1).transparent()) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0], local_pos[1] + 1, local_pos[2] + 1).transparent()) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1], local_pos[2] + 1).transparent()) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1], local_pos[2] + 1).transparent()) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] + 1, local_pos[2] + 1).transparent())
		b1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] + 1, local_pos[2] + 1).transparent())
		a1 = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] - 1, local_pos[1] - 1, local_pos[2] + 1).transparent())
		a = lightCvalue_;
	if (!chunk->checkblock(local_pos[0] + 1, local_pos[1] - 1, local_pos[2] + 1).transparent())
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, pz);
}


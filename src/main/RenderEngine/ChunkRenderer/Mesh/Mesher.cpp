#include <vector>
#include <iterator>
#include <chrono>
#include "../../../Minecraft/World/Level/Chunk/Chunk.h"
#include "../../../Minecraft/Core/Registry.h"

#include "TexMap.h"
#include "Mesher.h"

const int USE_SIZE = CHUNK_SIZE + 1;

const int USE_SIZE_2 = USE_SIZE * USE_SIZE;

using namespace std;

bool enableBlockShading_ = true;

char lightCvalue_ = 10;
char lightMvalue_ = 15;

int xDataBitOffset = 0;
int yDataBitOffset = 5;
int zDataBitOffset = 10;
int blockShadingBitOffset = 15;
int textureBitOffset = 20;


bool ChunkMesh::checkUseX(int x, int y, int z) {
	return useX[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseY(int x, int y, int z) {
	return useY[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseZ(int x, int y, int z) {
	return useZ[x * USE_SIZE_2 + y * USE_SIZE + z];
}


bool ChunkMesh::checkUseXN(int x, int y, int z) {
	return useXN[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseYN(int x, int y, int z) {
	return useYN[x * USE_SIZE_2 + y * USE_SIZE + z];
}
bool ChunkMesh::checkUseZN(int x, int y, int z) {
	return useZN[x * USE_SIZE_2 + y * USE_SIZE + z];
}

void ChunkMesh::clear() {
	vertices.clear();
	transparentVertices.clear();
}

void ChunkMesh::GreedyMeshing() {

}

void ChunkMesh::SmartGreedyMeshing() {
	//Check if it is empty
	if (chunk->Empty)
		return;

	SMesh = new BakedChunkData;

	//Gen Data

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				BlockID blockid = chunk->getBlock(x, y, z);

				if (BlockRegistry[blockid]->isSolid && (!BlockRegistry[blockid]->transparency)) {
					if (BlockRegistry[chunk->getBlock(x - 1, y, z)]->transparency)
						SaddSidenx(x, y, z, blockid);
					if (BlockRegistry[chunk->getBlock(x + 1, y, z)]->transparency)
						SaddSidepx(x, y, z, blockid);
					if (BlockRegistry[chunk->getBlock(x, y - 1, z)]->transparency)
						SaddSideny(x, y, z, blockid);
					if (BlockRegistry[chunk->getBlock(x, y + 1, z)]->transparency)
						SaddSidepy(x, y, z, blockid);
					if (BlockRegistry[chunk->getBlock(x, y, z - 1)]->transparency)
						SaddSidenz(x, y, z, blockid);
					if (BlockRegistry[chunk->getBlock(x, y, z + 1)]->transparency)
						SaddSidepz(x, y, z, blockid);
				}

				if (BlockRegistry[blockid]->isSolid && BlockRegistry[blockid]->transparency) {
					if (chunk->getBlock(x - 1, y, z) == AIR)
						SaddSidenx(x, y, z, blockid);
					if (chunk->getBlock(x + 1, y, z) == AIR)
						SaddSidepx(x, y, z, blockid);
					if (chunk->getBlock(x, y - 1, z) == AIR)
						SaddSideny(x, y, z, blockid);
					if (chunk->getBlock(x, y + 1, z) == AIR)
						SaddSidepy(x, y, z, blockid);
					if (chunk->getBlock(x, y, z - 1) == AIR)
						SaddSidenz(x, y, z, blockid);
					if (chunk->getBlock(x, y, z + 1) == AIR)
						SaddSidepz(x, y, z, blockid);

				}

				if (BlockRegistry[blockid]->isFluid) {
					if (BlockRegistry[chunk->getBlock(x, y + 1, z)]->transparency) {
						SaddSidepy(x, y, z, blockid);
					}
				}
			}
		}
	}
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

				VDATA[PX] = SMesh->extract(x, y, z).Vdata[PX];
				VDATA[NX] = SMesh->extract(x, y, z).Vdata[NX];
				VDATA[PY] = SMesh->extract(x, y, z).Vdata[PY];
				VDATA[NY] = SMesh->extract(x, y, z).Vdata[NY];
				VDATA[PZ] = SMesh->extract(x, y, z).Vdata[PZ];
				VDATA[NZ] = SMesh->extract(x, y, z).Vdata[NZ];

				bool transparent = SMesh->isTransparent(x,y,z);

				block_uv BLOCK;
				BLOCK.setBlockUVS(SMesh->GetBlockID(x,y,z));

				//PY
				if ((SMesh->extractVData(BMESH_ID, VDATA[PY]) != AIR)) {
					tpy_ = BLOCK.get_PY();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (x1 < CHUNK_SIZE) {
						if ((!checkUseY(x1, y, z)) && (VDATA[PY] == SMesh->extract(x1, y, z).Vdata[PY]) && (SMesh->isTransparent(x1, y + 1, z))) {
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
							if ((!checkUseY(dx, y, z1)) && (VDATA[PY] == SMesh->extract(dx, y, z1).Vdata[PY]) && (SMesh->isTransparent(dx, y + 1, z1))) {
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
					unsigned int quad = SMesh->extract(x, y, z).Vdata[PY];
					if (!((x0 - x1) == 0 || (z0 - z1) == 0))
						SGaddSidepy(ivec3(x0, y0, z0), ivec3(x1, y1, z1), false, x1 - x, z1 - z, SMesh->extractVData(BMESH_L0, quad), SMesh->extractVData(BMESH_L1, quad), SMesh->extractVData(L2, quad), SMesh->extractVData(L3, quad));
				}
				//PX
				if ((SMesh->extractVData(BMESH_ID, VDATA[PX]) != AIR)) {
					tpx_ = BLOCK.get_PX();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseX(x, y1, z)) && (VDATA[PX] == SMesh->extract(x, y1, z).Vdata[PX]) && (SMesh->isTransparent(x + 1, y1, z))) {
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
							if ((!checkUseX(x, dy, z1)) && (VDATA[PX] == SMesh->extract(x, dy, z1).Vdata[PX]) && (SMesh->isTransparent(x + 1, dy, z1))) {
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
					unsigned int quad = SMesh->extract(x, y, z).Vdata[PX];
					if (!((y0 - y1) == 0 || (z0 - z1) == 0))
						SGaddSidepx(ivec3(x0, y0, z0), ivec3(x1, y1, z1), false, y1 - y, z1 - z, SMesh->extractVData(BMESH_L3, quad), SMesh->extractVData(BMESH_L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L2, quad));
				}
				//PZ
				if ((SMesh->extractVData(BMESH_ID, VDATA[PZ]) != AIR)) {
					tpz_ = BLOCK.get_PZ();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseZ(x, y1, z)) && (VDATA[PZ] == SMesh->extract(x, y1, z).Vdata[PZ]) && (SMesh->isTransparent(x, y1, z + 1))) {
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
							if ((!checkUseZ(x1, dy, z)) && (VDATA[PZ] == SMesh->extract(x1, dy, z).Vdata[PZ]) && (SMesh->isTransparent(x1, dy, z + 1))) {
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
					unsigned int quad = SMesh->extract(x, y, z).Vdata[PZ];
					if (!((y0 - y1) == 0 || (x0 - x1) == 0))
						SGaddSidepz(ivec3(x0, y0, z0), ivec3(x1, y1, z1), false, x1 - x, y1 - y, SMesh->extractVData(BMESH_L2, quad), SMesh->extractVData(BMESH_L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L3, quad));
				}


				//NEGATIVES
				//NY
				if ((SMesh->extractVData(BMESH_ID, VDATA[NY]) != AIR)) {
					tny_ = BLOCK.get_NY();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (x1 < CHUNK_SIZE) {
						if ((!checkUseYN(x1, y, z)) && (VDATA[NY] == SMesh->extract(x1, y, z).Vdata[NY]) && (SMesh->isTransparent(x1, y - 1, z))) {
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
							if ((!checkUseYN(dx, y, z1)) && (VDATA[NY] == SMesh->extract(dx, y, z1).Vdata[NY]) && (SMesh->isTransparent(dx, y - 1, z1))) {
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
					unsigned int quad = SMesh->extract(x, y, z).Vdata[NY];
					if (!((x0 - x1) == 0 || (z0 - z1) == 0))
						SGaddSideny(ivec3(x0, y0, z0), ivec3(x1, y1, z1), false, x1 - x, z1 - z, SMesh->extractVData(BMESH_L0, quad), SMesh->extractVData(BMESH_L1, quad), SMesh->extractVData(L2, quad), SMesh->extractVData(L3, quad));
				}
				//NX
				if ((SMesh->extractVData(BMESH_ID, VDATA[NX]) != AIR)) {
					tnx_ = BLOCK.get_NX();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseXN(x, y1, z)) && (VDATA[NX] == SMesh->extract(x, y1, z).Vdata[NX]) && (SMesh->isTransparent(x - 1, y1, z))) {
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
							if ((!checkUseXN(x, dy, z1)) && (VDATA[NX] == SMesh->extract(x, dy, z1).Vdata[NX]) && (SMesh->isTransparent(x - 1, dy, z1))) {
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
					unsigned int quad = SMesh->extract(x, y, z).Vdata[NX];
					if (!((y0 - y1) == 0 || (z0 - z1) == 0))
						SGaddSidenx(ivec3(x0, y0, z0), ivec3(x1, y1, z1), false, y1 - y, z1 - z, SMesh->extractVData(BMESH_L3, quad), SMesh->extractVData(BMESH_L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L2, quad));
				}
				//PZ
				if ((SMesh->extractVData(BMESH_ID, VDATA[NZ]) != AIR)) {
					tnz_ = BLOCK.get_NZ();
					x0 = x;
					y0 = y;
					z0 = z;
					x1 = x;
					y1 = y;
					z1 = z;
					while (y1 < CHUNK_SIZE) {
						if ((!checkUseZN(x, y1, z)) && (VDATA[NZ] == SMesh->extract(x, y1, z).Vdata[NZ]) && (SMesh->isTransparent(x, y1, z - 1))) {
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
							if ((!checkUseZN(x1, dy, z)) && (VDATA[NZ] == SMesh->extract(x1, dy, z).Vdata[NZ]) && (SMesh->isTransparent(x1, dy, z - 1))) {
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
					unsigned int quad = SMesh->extract(x, y, z).Vdata[NZ];
					if (!((y0 - y1) == 0 || (x0 - x1) == 0))
						SGaddSidenz(ivec3(x0, y0, z0), ivec3(x1, y1, z1), false, y1 - y , x1 - x, SMesh->extractVData(BMESH_L2, quad), SMesh->extractVData(BMESH_L1, quad), SMesh->extractVData(L0, quad), SMesh->extractVData(L3, quad));
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

void ChunkMesh::SaddSideny(int x, int y, int z, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z)]->transparency) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z)]->transparency) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x, y - 1, z + 1)]->transparency) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x, y - 1, z - 1)]->transparency) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z + 1)]->transparency)
		a1 = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z - 1)]->transparency)
		a = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z - 1)]->transparency)
		b1 = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z + 1)]->transparency)
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, NY);
}
void ChunkMesh::SaddSidepy(int x, int y, int z, char B_ID) {
	int a = lightMvalue_;
	int b = lightMvalue_;
	int a1 = lightMvalue_;
	int b1 = lightMvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z)]->transparency) {
		a = lightCvalue_;
		b = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z)]->transparency) {
		a1 = lightCvalue_;
		b1 = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x, y + 1, z + 1)]->transparency) {
		a1 = lightCvalue_;
		b = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x, y + 1, z - 1)]->transparency) {
		b1 = lightCvalue_;
		a = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z + 1)]->transparency)
		a1 = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z - 1)]->transparency)
		a = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z - 1)]->transparency)
		b1 = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z + 1)]->transparency)
		b = lightCvalue_;
	SMesh->add(x, y, z, B_ID, a, b, a1, b1, PY);
}
void ChunkMesh::SaddSidenx(int x, int y, int z, char B_ID) {
	int br = lightMvalue_;
	int bl = lightMvalue_;
	int tl = lightMvalue_;
	int tr = lightMvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z)]->transparency) {
		br = lightCvalue_;
		bl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z)]->transparency) {
		tr = lightCvalue_;
		tl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x - 1, y, z + 1)]->transparency) {
		tr = lightCvalue_;
		br = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x - 1, y, z - 1)]->transparency) {
		bl = lightCvalue_;
		tl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z + 1)]->transparency)
		tr = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z - 1)]->transparency)
		tl = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z - 1)]->transparency)
		bl = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z + 1)]->transparency)
		br = lightCvalue_;
	SMesh->add(x, y, z, B_ID, br, bl, tr, tl, NX);
}
void ChunkMesh::SaddSidepx(int x, int y, int z, char B_ID) {
	int br = lightMvalue_;
	int bl = lightMvalue_;
	int tl = lightMvalue_;
	int tr = lightMvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z)]->transparency) {
		br = lightCvalue_;
		bl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z)]->transparency) {
		tr = lightCvalue_;
		tl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y, z + 1)]->transparency) {
		tr = lightCvalue_;
		br = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y, z - 1)]->transparency) {
		bl = lightCvalue_;
		tl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z + 1)]->transparency)
		tr = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z - 1)]->transparency)
		tl = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z - 1)]->transparency)
		bl = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z + 1)]->transparency)
		br = lightCvalue_;
	SMesh->add(x, y, z, B_ID, br, bl, tr, tl, PX); // (bl,br,tl,tr)
}
void ChunkMesh::SaddSidenz(int x, int y, int z, char B_ID) {
	int br = lightMvalue_;
	int bl = lightMvalue_;
	int tl = lightMvalue_;
	int tr = lightMvalue_;
	if (!BlockRegistry[chunk->getBlock(x, y - 1, z - 1)]->transparency) {
		br = lightCvalue_;
		bl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x, y + 1, z - 1)]->transparency) {
		tr = lightCvalue_;
		tl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y, z - 1)]->transparency) {
		tl = lightCvalue_;
		bl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x - 1, y, z - 1)]->transparency) {
		br = lightCvalue_;
		tr = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z - 1)]->transparency)
		tl = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z - 1)]->transparency)
		tr = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z - 1)]->transparency)
		br = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z - 1)]->transparency)
		bl = lightCvalue_;
	SMesh->add(x, y, z, B_ID, br, bl, tl, tr, NZ); // (br,bf,tf,tr)
}
void ChunkMesh::SaddSidepz(int x, int y, int z, char B_ID) {
	int br = lightMvalue_;
	int bl = lightMvalue_;
	int tl = lightMvalue_;
	int tr = lightMvalue_;
	if (!BlockRegistry[chunk->getBlock(x, y - 1, z + 1)]->transparency) {
		br = lightCvalue_;
		bl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x, y + 1, z + 1)]->transparency) {
		tr = lightCvalue_;
		tl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y, z + 1)]->transparency) {
		tl = lightCvalue_;
		bl = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x - 1, y, z + 1)]->transparency) {
		br = lightCvalue_;
		tr = lightCvalue_;
	}
	if (!BlockRegistry[chunk->getBlock(x + 1, y + 1, z + 1)]->transparency)
		tl = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y + 1, z + 1)]->transparency)
		tr = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x - 1, y - 1, z + 1)]->transparency)
		br = lightCvalue_;
	if (!BlockRegistry[chunk->getBlock(x + 1, y - 1, z + 1)]->transparency)
		bl = lightCvalue_;
	SMesh->add(x, y, z, B_ID, br, bl, tl, tr, PZ);
}
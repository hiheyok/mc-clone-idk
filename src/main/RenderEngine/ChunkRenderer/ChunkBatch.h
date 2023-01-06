#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "../../Utils/LogUtils.h"
#include "../Camera/camera.h"
#include "../frustum/frustum.h"
#include "../../Utils/MathHelper.h"
#include "../../Utils/Math/ChunkID/ChunkID.h"
#include "../../Minecraft/World/Level/Chunk/Chunk.h"
#include "../OpenGL/Buffers/Buffer.h"
#include <unordered_map>

struct DrawCommandIndirect {
	unsigned int  count;
	unsigned int  instanceCount;
	unsigned int  first;
	unsigned int  baseInstance;
};

struct DataBufferAddress {
	size_t offset = 0;
	size_t size = 0;
	int x, y, z;
};


class ChunkDrawBatch {
public:

	void SetupBuffers();

	void Reset();

	void GenDrawCommands(int RenderDistance);

	void AddChunkVertices(std::vector<unsigned int> Data, int x, int y, int z);

	void DeleteChunkVertices(CHUNK_ID ChunkID);
private:
	Camera* camera;
	CFrustum* Frustum;
	Buffer VBO, IBO, SSBO;
	VertexArray Array;
	size_t MaxBufferSize = 10000000;
	size_t MemoryUsage = 0;

	bool UpdateCommands = false;

	std::unordered_map<size_t, int> OffsetRenderIndexLookup;
	std::unordered_map<long long int, size_t> RenderListOffsetLookup;
	std::vector<GLint> ChunkShaderPos;
	std::vector<DataBufferAddress> RenderList;
	std::vector<DrawCommandIndirect> DrawCommands;
};
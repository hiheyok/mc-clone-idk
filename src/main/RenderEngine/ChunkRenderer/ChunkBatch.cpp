#include "ChunkBatch.h"

void ChunkDrawBatch::SetupBuffers() {
	VBO.SetType(GL_ARRAY_BUFFER);
	IBO.SetType(GL_DRAW_INDIRECT_BUFFER);
	SSBO.SetType(GL_SHADER_STORAGE_BUFFER);

	VBO.SetUsage(GL_STATIC_DRAW);
	IBO.SetUsage(GL_DYNAMIC_DRAW);
	SSBO.SetUsage(GL_DYNAMIC_DRAW);

	Array.Bind();

	VBO.SetMaxSize(MaxBufferSize);
	VBO.InitializeData();
	IBO.SetMaxSize(MaxBufferSize);
	IBO.InitializeData();

	VBO.Bind();
	Array.EnableAttriPTR(0, 1, GL_FLOAT, GL_FALSE, 2, 0);
	Array.EnableAttriPTR(1, 1, GL_FLOAT, GL_FALSE, 2, 1);
	VBO.Unbind();
	Array.Unbind();

	SSBO.SetMaxSize(MaxBufferSize);
	SSBO.InitializeData();
}

void ChunkDrawBatch::Reset() {
	VBO.ResetBuffer();
	IBO.ResetBuffer();
	SSBO.ResetBuffer();
	Array.ResetArray();
	SetupBuffers();
}

void ChunkDrawBatch::GenDrawCommands(int RenderDistance) {
	if (!UpdateCommands)
		return;

	DrawCommands.clear();
	ChunkShaderPos.clear();

	ivec3 Position(floor(camera->Position.x / 16), floor(camera->Position.y / 16), floor(camera->Position.z / 16));

	int Index = 1;

	for (int i = 0; i < RenderList.size(); i++) {
		DataBufferAddress data = RenderList[(RenderList.size() - 1) - i];
		if (FindDistanceNoSqrt(data.x, data.y, data.z, Position.x, Position.y, Position.z) < pow(RenderDistance, 2)) {
			if (Frustum->SphereInFrustum((float)data.x * 16, (float)data.y * 16, (float)data.z * 16, (float)32)) {
				DrawCommandIndirect cmd;
				cmd.count = (unsigned int)data.size / (sizeof(unsigned int) * 2);
				cmd.instanceCount = 1;
				cmd.first = (unsigned int)data.offset / (sizeof(unsigned int) * 2);
				cmd.baseInstance = Index;
				DrawCommands.push_back(cmd);
				ChunkShaderPos.emplace_back(data.x);
				ChunkShaderPos.emplace_back(data.y);
				ChunkShaderPos.emplace_back(data.z);
				Index++;
			}
		}

	}
	SSBO.InsertSubData(0, ChunkShaderPos.size() * sizeof(int), ChunkShaderPos.data());
	IBO.InsertSubData(0, DrawCommands.size() * sizeof(DrawCommandIndirect), DrawCommands.data());
}

void ChunkDrawBatch::AddChunkVertices(std::vector<unsigned int> Data, int x, int y, int z) {
	unsigned long long int ChunkID = getChunkID(x, y, z);
	size_t DataSize = Data.size() * sizeof(unsigned int);
	if (DataSize == 0) { return; }
	DataBufferAddress RenderingData;
	RenderingData.x = x;
	RenderingData.y = y;
	RenderingData.z = z;

	if (RenderList.size() == 0) {
		RenderingData.offset = 0;
		RenderingData.size = DataSize;
		MemoryUsage += DataSize;
		VBO.InsertSubData(RenderingData.offset, Data.size(), &Data);
		RenderList.insert(RenderList.begin(), RenderingData);
		RenderListOffsetLookup[ChunkID] = 0;
		OffsetRenderIndexLookup[0] = 0;
		UpdateCommands = true;
		return;
	}
	if (RenderList.back().offset + RenderList.back().size + MemoryUsage < MaxBufferSize) {
		RenderingData.offset = RenderList.back().offset + RenderList.back().size;
		RenderingData.size = DataSize;
		MemoryUsage += DataSize;
		VBO.InsertSubData(RenderingData.offset, Data.size(), &Data);
		RenderList.emplace_back(RenderingData);
		RenderListOffsetLookup[ChunkID] = RenderingData.offset;
		OffsetRenderIndexLookup[RenderingData.offset] = Data.size() - 1;
		UpdateCommands = true;
		return;
	}
	for (int i = 0; i < RenderList.size(); i++) {
		if (RenderList[i].offset + RenderList[i].size + DataSize < RenderList[i + 1].offset) {
			RenderingData.offset = RenderList[i].offset + RenderList[i].size;
			RenderingData.size = DataSize;
			MemoryUsage += DataSize;
			VBO.InsertSubData(RenderingData.offset, Data.size(), &Data);
			RenderList.insert(RenderList.begin() + i + 1, RenderingData);
			RenderListOffsetLookup[ChunkID] = RenderingData.offset;
			OffsetRenderIndexLookup[RenderingData.offset] = i;
			UpdateCommands = true;
			return;
		}
	}
	getLogger()->LogError("Chunk Renderer", "GPU Out of Memory: " + std::to_string(MemoryUsage));
}

void ChunkDrawBatch::DeleteChunkVertices(CHUNK_ID ChunkID) {
	if (RenderListOffsetLookup.count(ChunkID)) {
		size_t index = OffsetRenderIndexLookup[RenderListOffsetLookup[ChunkID]];
		MemoryUsage -= RenderList[index].size;
		RenderList.erase(RenderList.begin() + index);
		OffsetRenderIndexLookup.erase(RenderListOffsetLookup[ChunkID]);
		RenderListOffsetLookup.erase(ChunkID);
	}
}
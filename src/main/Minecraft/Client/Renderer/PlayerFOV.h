#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "../../../RenderEngine/ChunkRenderer/ChunkRenderer.h"
#include "../../World/World.h"

class PlayerFOV {
public:
	void JoinWorld(World* world);
	void LeaveWorld();

	void Render();
	
private:
	void RenderWorld();
	std::deque<Chunk> ChunksToRender;
	ChunkRenderer TerrainRenderer;
};


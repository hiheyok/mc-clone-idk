#pragma once

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


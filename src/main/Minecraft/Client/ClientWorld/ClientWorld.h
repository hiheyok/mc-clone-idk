#pragma once

#include "../../World/Level/Chunk/Chunk.h"
#include "../../../Utils/MutithreadedData.h"

#include <deque>
#include <unordered_map>
#include <thread>
#include "../../World/Entity/Entities/Player.h"
#include "../../../RenderEngine/Camera/camera.h"
#include "../../../RenderEngine/ChunkRenderer/ChunkRenderer.h"
class ClientWorld {
public:

	const unsigned int TPS = 20;

	void AddChunkServer(Chunk chunk);
	void ClientWorldMainLoop();
	void Start(GLFWwindow* window_);
	void UpdatePlayer(double delta, std::unordered_map<char, bool> Keys, glm::vec2 MouseMovement);  //update player stuff pos rot 
	void Render();
	void PrepareRenderer();

	double MouseSens = 0.25;

private:
	
	void DumpRenderQueuedData();
	
	void AddChunkToRenderQueue(Chunk chunk);
	void UpdateChunks();

	ChunkRenderer* TerrrainRenderer = nullptr;
	Entity* player = nullptr;
	Camera camera;

	AsyncDeque<Chunk> ChunkAddQueue;
//	AsyncDeque<???> BlockUpdates; //Work on later
	AsyncHashMapClass<CHUNK_ID, Chunk> ChunkCache;
	AsyncHashMapClass<CHUNK_ID, Chunk> RenderChunkUpdateQueue;

	bool stop = false;

	std::thread ClientWorldThread;
};
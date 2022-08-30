#pragma once

#include "../../World/Level/Chunk/Chunk.h"
#include "../../../Utils/MutithreadedData.h"

#include <deque>`
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
	void Start();
	void UpdatePlayer(double delta);  //update player stuff pos rot 
	void Render();

	double MouseSens = 1.0;

private:
	
	void PrepareRenderer();
	void AddChunkToRenderQueue(Chunk chunk);
	void UpdateChunks();

	Entity* player;
	Camera camera;

	AsyncDeque<Chunk> ChunkAddQueue;
//	AsyncDeque<???> BlockUpdates; //Work on later
	AsyncHashMapClass<CHUNK_ID, Chunk> ChunkCache;
	AsyncHashMapClass<CHUNK_ID, mesher> RenderChunkUpdateQueue;

	bool stop = false;

	std::thread ClientWorldThread;

	std::unordered_map<char, bool> KeysInputs;
	glm::vec2 MouseMovement = glm::vec2(0.0,0.0);
};
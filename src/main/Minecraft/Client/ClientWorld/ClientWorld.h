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
	void Tick();

	glm::vec3 GetPlayerPos();
	glm::vec3 GetPlayerSpeed();

	double Gravity = 10.0;
	double MouseSens = 0.25;

private:
	
	bool TestIfEntityOnGround(Entity* ENTITY);
	bool TestIfEntityTouchBlockXP(Entity* ENTITY);
	bool TestIfEntityTouchBlockZP(Entity* ENTITY);

	bool TestIfEntityTouchBlockXN(Entity* ENTITY);
	bool TestIfEntityTouchBlockZN(Entity* ENTITY);
	void MoveEntity(Entity* ENTITY, double x, double y, double z);
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
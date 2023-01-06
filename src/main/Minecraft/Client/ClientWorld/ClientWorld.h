#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
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
	void AddMeshWorker();

	glm::vec3 GetPlayerPos();
	glm::vec3 GetPlayerSpeed();

	double Gravity = 10.0;
	double MouseSens = 0.25;

private:
	
	bool TestIfEntityOnGround(Entity* ENTITY);
	bool TestIfEntityTouchBlockYP(Entity* ENTITY);

	bool TestIfEntityTouchBlockXP(Entity* ENTITY);
	bool TestIfEntityTouchBlockZP(Entity* ENTITY);

	bool TestIfEntityTouchBlockXN(Entity* ENTITY);
	bool TestIfEntityTouchBlockZN(Entity* ENTITY);
	void MoveEntity(Entity* ENTITY, double x, double y, double z);
	void DumpRenderQueuedData();
	
	void AddChunkToRenderQueue(Chunk chunk);
	void UpdateChunks();
	void MesherWorker();

	ChunkRenderer* TerrrainRenderer = nullptr;
	Entity* player = nullptr;
	Camera camera;

	concurrency::concurrent_queue<Chunk> ChunkAddQueue;
	Concurrency::concurrent_priority_queue<long long int> ChunkMeshQueue;
//	AsyncDeque<???> BlockUpdates; //Work on later
	AsyncHashMap<CHUNK_ID, Chunk> ChunkCache;

	long long unsigned CPU_TIME = 0;

	bool stop = false;

	std::deque<std::thread> MeshWorkers;
	std::thread ClientWorldThread;

};
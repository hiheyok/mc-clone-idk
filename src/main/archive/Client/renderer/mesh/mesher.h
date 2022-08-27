#ifndef MESHER_H
#define MESHER_H

#include "../../../Server/World/Chunk/Chunk.h"
#include "../../../Server/World/World.h"
#include "../../../utils/MathHelper.h"
#include "../../../utils/functions/clock/Clock.h"
#include "../../../utils/functions/logging/log.h"
#include <unordered_map>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <glm/vec3.hpp>
#include <string>




class MesherV2 {
public:

	MesherV2(World* world) {
		WorldData = world;
	}

	World* WorldData = nullptr;
	int maxBakesPerSecond = 0;
	std::vector<glm::ivec3> CreateRenderingDataQueue;

	ChunkMesh* returnRenderingData(int x, int y, int z) {
		/* if (inUseRender) {
			 precisePause(QUEUE_TIME);
		 }*/

		render_m.lock();
		ChunkMesh* tmp = &RenderingInfoMap[getChunkID(x, y, z)];
		render_m.unlock();
		return tmp;
	}

	bool checkIfChunkRenderingDataLoaded(int x, int y, int z) {
		/* if (inUseRender) {
			 precisePause(QUEUE_TIME);
		 }*/
		render_m.lock();
		if (RenderingInfoMap.count(getChunkID(x, y, z))) {
			render_m.unlock();
			return true;
		}
		else {
			render_m.unlock();
			return false;
		}

	}

	bool checkIfChunkRenderingDataLoadedUnlocked(int x, int y, int z) {
		/* if (inUseRender) {
			 precisePause(QUEUE_TIME);
		 }*/
		if (RenderingInfoMap.count(getChunkID(x, y, z))) {
			return true;
		}
		else {
			return false;
		}

	}

	bool checkIfChunkRenderingDataLoaded(long long int chunkID) {
		/* if (inUseRender) {
			 precisePause(QUEUE_TIME);
		 }*/
		render_m.lock();
		if (RenderingInfoMap.count(chunkID)) {
			render_m.unlock();
			return true;
		}
		else {
			render_m.unlock();
			return false;
		}

	}

	bool checkIfChunkRenderingDataLoadedUnlocked(long long int chunkID) {
		/* if (inUseRender) {
			 precisePause(QUEUE_TIME);
		 }*/
		if (RenderingInfoMap.count(chunkID)) {
			return true;
		}
		else {
			return false;
		}

	}

	void unloadRenderingData(glm::ivec3 pos) {
		/*if (inUseRender) {
			precisePause(QUEUE_TIME);
		}*/
		render_m.lock();
		verticesSize = verticesSize - RenderingInfoMap[getChunkID(pos)].vertices.size() - RenderingInfoMap[getChunkID(pos)].transparentVertices.size();
		
		RenderingInfoMap[getChunkID(pos)].delete_();
		RenderingInfoMap.erase(getChunkID(pos));
		render_m.unlock();
	}

	void unloadRenderingDataUnlocked(glm::ivec3 pos) {
		/*if (inUseRender) {
			precisePause(QUEUE_TIME);
		}*/
		verticesSize = verticesSize - RenderingInfoMap[getChunkID(pos)].vertices.size() - RenderingInfoMap[getChunkID(pos)].transparentVertices.size();

		MeshQueueHashMap.erase(getChunkID(pos));
		RenderingInfoMap[getChunkID(pos)].delete_();
		RenderingInfoMap.erase(getChunkID(pos));

	}

	void stop() {
		run = false;
		for (int i = 0; i < amountOfThreads; i++) {
			threads[i].join();
		}
		mainMeshQueue.join();

	}

	void startWorkers(int num_threads) {

		amountOfThreads = num_threads;

		threads.resize(num_threads);

		for (int i = 0; i < num_threads; ++i)
		{
			threads[i] = std::thread(&MesherV2::MeshWorkerThread, this);
		}

		mainMeshQueue = std::thread(&MesherV2::MeshQueueThread, this);
	}

	long long int getRenderingMemSize() {
		return verticesSize * sizeof(unsigned int);
	}

	glm::ivec3 playerPos = glm::ivec3(0, 0, 0);

	std::mutex render_m;
	std::mutex mesh_queue_m;
	
	std::vector<glm::ivec3> MeshQueue;

private:

	bool run = true;

	
	

	void MeshWorkerThread();
	void MeshQueueThread();

	int amountOfThreads = 0;

	std::thread mainMeshQueue;

	
	std::vector<std::thread> threads;



	std::unordered_map< long long int, bool > MeshQueueHashMap;
	std::unordered_map< long long int, ChunkMesh > RenderingInfoMap;

	int verticesSize = 0;
};

#endif
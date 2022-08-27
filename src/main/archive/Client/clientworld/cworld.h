#pragma once
#include "../../Server/server.h"
#include <unordered_map>
#include "../../Server/world/chunk/Chunk.h"
#include "../../utils/functions/clock/Clock.h"
#include "../renderer/gui/Settings/Settings.h"
#include <thread>
#include <deque>
#include <chrono>
class ClientWorld {
public:

	void init(Server* cserver) { // attach server address
		server = cserver;
		cworldThread = std::thread(&ClientWorld::ClientWorldLoop, this);
	}

	void ClientWorldLoop() {

		std::cout << "ClientWorld : Thread ID: " << std::this_thread::get_id() << "\n";

		while (true) {

			int render_distance = getRENDER_DISTANCE();
			int render_height = getRENDER_HEIGHT();

			auto time0 = std::chrono::high_resolution_clock::now();
			for (int x = (playerpos.x / 16) - render_distance; x <= (playerpos.x / 16) + render_distance; x++) {
				for (int y = (playerpos.y / 16) - render_height; y <= (playerpos.y / 16) + render_height; y++) {
					if (y < 0) {
						y = 0;
					}
					for (int z = (playerpos.z / 16) - render_distance; z <= (playerpos.z / 16) + render_distance; z++) {
						if (chunkcache.count(getChunkID(x, y, z))) {

						}
						else {
							REQUEST req;
							req.type = CHUNK;
							req.x = x;
							req.y = y;
							req.z = z;
							req.ADDRESS = &packetsQueued;
							server->request(req);
						}
					}
				}
			}

			processPackets();
			processQueuedJobs();
			ClearMem();


			std::chrono::duration<double, std::milli> looptime = std::chrono::high_resolution_clock::now() - time0;

			//std::cout << "Client World Tick: " << looptime.count() << "ms\n";

			timerSleep(0.05 - (looptime.count() / 1000.0));
		}
	}

	void processQueuedJobs() {
	//	if (!ChunkCacheBusy && !RenderQueueBusy ) {
			while (ChunkAddQueue.size() != 0) {
				AddChunk(ChunkAddQueue.front());
				ChunkAddQueue.pop_front();
			}
			while (ChunkRemoveQueue.size() != 0) {
				UnloadChunk(ChunkRemoveQueue.front());
				ChunkRemoveQueue.pop_front();
			}
			if (!RenderQueueBusy) {
				RenderQueueBusy = true;
				RenderingQueueLock.lock();
				for (int index = 0; index < MaxChunkLoadingSpeed; index++) {
					if (ChunkRenderingQueueCache.size() != 0) {
						AddChunkRenderingQueue.emplace_back(ChunkRenderingQueueCache.front());
						ChunkRenderingQueueCache.pop_front();
					} else {
						break;
					}
				}
				RenderingQueueLock.unlock();
				RenderQueueBusy = false;
				
			}
	//	}
	}

	void processPackets() {
		//std::cout << "test\n";
		for (int i = 0; i < packetsQueued.size(); i++) {

			PACKET pac = packetsQueued[i];



			if (pac.type == CHUNK) {

				glm::ivec3 pos = pac.chunk->pos;

				if (!chunkcache.count(getChunkID(pos.x, pos.y, pos.z))) {

					AddChunk(*pac.chunk);

					packetsQueued.erase(packetsQueued.begin() + i);
					DeleteQueue.emplace_back(pac.chunk);
				}
				else {
					packetsQueued.erase(packetsQueued.begin() + i);
					DeleteQueue.emplace_back(pac.chunk);
				}
			}


		}
	}

	void AddChunk(Chunk chunk) {
		if (ChunkCacheBusy) {
			ChunkAddQueue.emplace_back(chunk);
			return;
		}

		ChunkCacheBusy = true;
		glm::ivec3 pos = chunk.pos;

		chunkcache[getChunkID(pos.x, pos.y, pos.z)] = chunk;
	//	CHUNKQUEUELOCK.lock();
		//Check and Update Chunk Neighbors
		if (chunkcache.count(getChunkID(pos.x, pos.y + 1, pos.z))) {
			chunkcache[getChunkID(pos.x, pos.y + 1, pos.z)].setNeighborNY(&chunkcache[getChunkID(pos.x, pos.y, pos.z)]);
			chunkcache[getChunkID(pos.x, pos.y, pos.z)].setNeighborPY(&chunkcache[getChunkID(pos.x, pos.y + 1, pos.z)]);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y + 1, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x, pos.y - 1, pos.z))) {
			chunkcache[getChunkID(pos.x, pos.y - 1, pos.z)].setNeighborPY(&chunkcache[getChunkID(pos.x, pos.y, pos.z)]);
			chunkcache[getChunkID(pos.x, pos.y, pos.z)].setNeighborNY(&chunkcache[getChunkID(pos.x, pos.y - 1, pos.z)]);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y - 1, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x + 1, pos.y, pos.z))) {
			chunkcache[getChunkID(pos.x + 1, pos.y, pos.z)].setNeighborNX(&chunkcache[getChunkID(pos.x, pos.y, pos.z)]);
			chunkcache[getChunkID(pos.x, pos.y, pos.z)].setNeighborPX(&chunkcache[getChunkID(pos.x + 1, pos.y, pos.z)]);
			AddMeshData(chunkcache[getChunkID(pos.x + 1, pos.y, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x - 1, pos.y, pos.z))) {
			chunkcache[getChunkID(pos.x - 1, pos.y, pos.z)].setNeighborPX(&chunkcache[getChunkID(pos.x, pos.y, pos.z)]);
			chunkcache[getChunkID(pos.x, pos.y, pos.z)].setNeighborNX(&chunkcache[getChunkID(pos.x - 1, pos.y, pos.z)]);
			AddMeshData(chunkcache[getChunkID(pos.x - 1, pos.y, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x, pos.y, pos.z + 1))) {
			chunkcache[getChunkID(pos.x, pos.y, pos.z + 1)].setNeighborNZ(&chunkcache[getChunkID(pos.x, pos.y, pos.z)]);
			chunkcache[getChunkID(pos.x, pos.y, pos.z)].setNeighborPZ(&chunkcache[getChunkID(pos.x, pos.y, pos.z + 1)]);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y, pos.z + 1)]);
		}
		if (chunkcache.count(getChunkID(pos.x, pos.y, pos.z - 1))) {
			chunkcache[getChunkID(pos.x, pos.y, pos.z - 1)].setNeighborPZ(&chunkcache[getChunkID(pos.x, pos.y, pos.z)]);
			chunkcache[getChunkID(pos.x, pos.y, pos.z)].setNeighborNZ(&chunkcache[getChunkID(pos.x, pos.y, pos.z - 1)]);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y, pos.z - 1)]);
		}

		AddMeshData(chunk);
	//	CHUNKQUEUELOCK.unlock();
		ChunkCacheBusy = false;
	}

	void UnloadChunk(glm::ivec3 pos) {

		if (ChunkCacheBusy) {
			ChunkRemoveQueue.emplace_back(pos);
			return;
		}
		ChunkCacheBusy = true;
		chunkcache.erase(getChunkID(pos.x, pos.y, pos.z));
	//	CHUNKQUEUELOCK.lock();
		//Check and Update Chunk Neighbors
		RemoveChunkRenderingQueue.emplace_back(getChunkID(pos.x, pos.y, pos.z));
		if (chunkcache.count(getChunkID(pos.x, pos.y + 1, pos.z))) {
			chunkcache[getChunkID(pos.x, pos.y + 1, pos.z)].setNeighborNY(nullptr);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y + 1, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x, pos.y - 1, pos.z))) {
			chunkcache[getChunkID(pos.x, pos.y - 1, pos.z)].setNeighborPY(nullptr);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y - 1, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x + 1, pos.y, pos.z))) {
			chunkcache[getChunkID(pos.x + 1, pos.y, pos.z)].setNeighborNX(nullptr);
			AddMeshData(chunkcache[getChunkID(pos.x + 1, pos.y, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x - 1, pos.y, pos.z))) {
			chunkcache[getChunkID(pos.x - 1, pos.y, pos.z)].setNeighborPX(nullptr);
			AddMeshData(chunkcache[getChunkID(pos.x - 1, pos.y, pos.z)]);
		}
		if (chunkcache.count(getChunkID(pos.x, pos.y, pos.z + 1))) {
			chunkcache[getChunkID(pos.x, pos.y, pos.z + 1)].setNeighborNZ(nullptr);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y, pos.z + 1)]);
		}
		if (chunkcache.count(getChunkID(pos.x, pos.y, pos.z - 1))) {
			chunkcache[getChunkID(pos.x, pos.y, pos.z - 1)].setNeighborPZ(nullptr);
			AddMeshData(chunkcache[getChunkID(pos.x, pos.y, pos.z - 1)]);
		}
	//	CHUNKQUEUELOCK.unlock();
		ChunkCacheBusy = false;
	}

	void ClearMem() {
		while (!DeleteQueue.empty()) {
			//delete DeleteQueue.front();
			DeleteQueue.pop_front();
		}
	}

	void AddMeshData(Chunk chunk) {
		ChunkMesh mesh;
		mesh.chunk = &chunk;
		mesh.SmartGreedyMeshing();
		mesh.pos = chunk.pos;
		ChunkRenderingQueueCache.emplace_back(mesh);
	}

	bool RenderQueueBusy = false;
	bool ChunkCacheBusy = false;

	std::deque<Chunk> ChunkAddQueue;
	std::deque<glm::ivec3> ChunkRemoveQueue;

	std::deque<ChunkMesh> AddChunkRenderingQueue;
	std::deque<long long int> RemoveChunkRenderingQueue;

	std::mutex RenderingQueueLock;

	std::unordered_map<long long int, Chunk> chunkcache;

	glm::vec3 playerpos = glm::vec3(0, 0, 0);

	int MaxChunkLoadingSpeed = 1000;

private:

	std::deque<ChunkMesh> ChunkRenderingQueueCache;
	std::deque<void*> DeleteQueue;

	Server* server;

	std::thread cworldThread;
	std::vector<PACKET> packetsQueued;


	std::unordered_map<long long int, REQUEST> requestCooldown;
};
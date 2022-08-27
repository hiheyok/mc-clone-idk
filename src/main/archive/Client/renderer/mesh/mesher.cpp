//#include "mesher.h"
//#include "../gui//Settings/Settings.h"
//
//using namespace glm;
//using namespace std;
//
//void MesherV2::MeshWorkerThread() {
//
//	std::cout << "MeshGenerator : Thread ID: " << std::this_thread::get_id() << "\n";
//
//	vector<ivec3> l_buffer;
//
//	l_buffer.reserve(262144);
//
//	while (run) {
//
//		if (maxBakesPerSecond != 0) {
//
//
//
//			mesh_queue_m.lock();
//			for (int index = 0; index < maxBakesPerSecond; index++) {
//
//
//				if (MeshQueue.size() != 0) {
//
//
//					ivec3 cpos = MeshQueue[0];
//					MeshQueue.erase(MeshQueue.begin());
//
//					l_buffer.push_back(cpos);
//
//				}
//				else { break; }
//				if (!run) { break; }
//			}
//			mesh_queue_m.unlock();
//
//
//			for (int loops = 0; loops < l_buffer.size(); loops++) {
//
//				ivec3 cpos = l_buffer[0];
//				l_buffer.erase(l_buffer.begin());
//
//				WorldData->chunk_m.lock();
//				Chunk tmpChunk = WorldData->loadedChunks[getChunkID(cpos.x, cpos.y, cpos.z)];
//
//				Chunk tmpChunk0 = WorldData->loadedChunks[getChunkID(cpos.x - 1, cpos.y, cpos.z)];
//				Chunk tmpChunk1 = WorldData->loadedChunks[getChunkID(cpos.x + 1, cpos.y, cpos.z)];
//				Chunk tmpChunk2 = WorldData->loadedChunks[getChunkID(cpos.x, cpos.y - 1, cpos.z)];
//				Chunk tmpChunk3 = WorldData->loadedChunks[getChunkID(cpos.x, cpos.y + 1, cpos.z)];
//				Chunk tmpChunk4 = WorldData->loadedChunks[getChunkID(cpos.x, cpos.y, cpos.z - 1)];
//				Chunk tmpChunk5 = WorldData->loadedChunks[getChunkID(cpos.x, cpos.y, cpos.z + 1)];
//				WorldData->chunk_m.unlock();
//				tmpChunk.setNeighborNX(&tmpChunk0);
//				tmpChunk.setNeighborPX(&tmpChunk1);
//				tmpChunk.setNeighborNY(&tmpChunk2);
//				tmpChunk.setNeighborPY(&tmpChunk3);
//				tmpChunk.setNeighborNZ(&tmpChunk4);
//				tmpChunk.setNeighborPZ(&tmpChunk5);
//
//				ChunkMesh newMesh;
//				newMesh.chunk = &tmpChunk;
//				newMesh.cpos = tmpChunk.getcpos();
//
//				newMesh.SmartGreedyMeshing();
//
//				MeshQueueHashMap.erase(getChunkID(cpos));
//
//				verticesSize = verticesSize + newMesh.vertices.size();
//
//				if (!run) { break; }
//
//				if ((newMesh.transparentVertices.size() + newMesh.vertices.size()) != 0) {
//					newMesh.init = true;
//
//					render_m.lock();
//					RenderingInfoMap[getChunkID(cpos)] = newMesh;
//					render_m.unlock();
//				}
//			//	mesh_queue_m.lock();
//				CreateRenderingDataQueue.push_back(cpos);
//			//	mesh_queue_m.unlock();
//			}
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
//	}
//}
//
//void MesherV2::MeshQueueThread() {
//	std::vector<glm::ivec3> tmp;
//
//	tmp.reserve(1000000);
//
//	std::cout << "Gen Baking Queue : Thread ID: " << std::this_thread::get_id() << "\n";
//
//	while (run) {
//
//		auto time0 = chrono::high_resolution_clock::now();
//
//		int render_d = getRENDER_DISTANCE();
//		int render_h = getRENDER_HEIGHT();
//
//
//
//		vec3 cm = vec3(playerPos.x / CHUNK_SIZE, playerPos.y / CHUNK_SIZE, playerPos.z / CHUNK_SIZE);
//
//		float c_constant = 1.0f / (float)CHUNK_SIZE;
//
//		for (int x = (int)cm.x - render_d; x < (int)cm.x + render_d; x++) {
//			int cmy_ = (int)cm.y - render_h;
//			if (cmy_ < 0)
//				cmy_ = 0;
//			if (!run) { break; }
//			for (int y = cmy_; y < (int)cm.y + render_h; y++) {
//				if (!run) { break; }
//				for (int z = (int)cm.z - render_d; z < (int)cm.z + render_d; z++) {
//					//if (FindDistance(x, z, (int)(playerPos.x * c_constant), (int)(playerPos.z * c_constant)) <= render_d) {
//
//					if (!run) { break; }
//
//
//				}
//			}
//		}
//		mesh_queue_m.lock();
//		MeshQueue.insert(MeshQueue.begin(), tmp.begin(), tmp.end());
//		maxBakesPerSecond = (int)MeshQueue.size() / amountOfThreads;
//		mesh_queue_m.unlock();
//
//		tmp.clear();
//
//		std::chrono::duration<double, std::milli> nano = std::chrono::high_resolution_clock::now() - time0;
//
//		//	std::cout << "Bake: " << nano.count() << "\n";
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//	}
//}
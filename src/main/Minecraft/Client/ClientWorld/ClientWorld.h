#pragma once

#include "../../World/Level/Chunk/Chunk.h"
#include "../../../Utils/MutithreadedData.h"

#include <deque>`
#include <unordered_map>
#include <thread>

class ClientWorld {
public:

	const unsigned int TPS = 20;

	void AddChunkServer(Chunk chunk);
	void ClientWorldMainLoop();
	void Start();
private:

	void UpdateChunks();

	AsyncDeque<Chunk> ChunkAddQueue;
//	AsyncDeque<???> BlockUpdates; //Work on later
	AsyncHashMapClass<CHUNK_ID, Chunk> ChunkCache;

	bool stop = false;

	std::thread ClientWorldThread;

	std::unordered_map<char, bool> KeysInputs;
};
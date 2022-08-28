#pragma once

#include "../../World/Level/Chunk/Chunk.h"

#include <deque>`
#include <unordered_map>

class ClientWorld {
public:
	void AddChunkServer(Chunk chunk);
	void ClientWorldMainLoop();
private:
	std::deque<Chunk> ChunkQueue;
	std::unordered_map<long long int, Chunk> ChunkCache;
};
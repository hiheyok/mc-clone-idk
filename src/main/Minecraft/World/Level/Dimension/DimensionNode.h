#pragma once
#include "../Chunk/ChunkMap.h"
#include "DimensionProperties.h"
#include "../TickSync/TickSync.h"
#include "../../../../Utils/LogUtils.h"
#include <thread>

class DimensionNode {
public:
	DimensionNode() {

	}

	void Initialize(DimensionProperties* Properties, std::unordered_map<ID, DimensionNode*>* Nodes, std::unordered_map<CHUNK_ID, ID>* ChunkNodeLocation, ID NodeID_) {
		DProperties = Properties;
		DNodes = Nodes;
		ChunkDNodeLocation = ChunkNodeLocation;
		NodeID = NodeID_;
	}

	~DimensionNode() {
		if (!stop) {
			stop = true;
			NodeThread.join();
			DProperties = nullptr;
		}
		getLogger()->LogDebug("Dimension Node", "Destroyed Node ID: " + std::to_string(NodeID));
	}

	void AddTickingChunk(Chunk* chunk) {
		WorkingChunks[chunk->ChunkID] = chunk;
	}

	void Run();

	void Stop();

	void Tick();

	ID NodeID = NULL;
private:
	DimensionProperties* DProperties;
	std::unordered_map<ID, DimensionNode*>* DNodes;
	std::unordered_map<CHUNK_ID, ID>* ChunkDNodeLocation;
	std::unordered_map<CHUNK_ID, Chunk*> WorkingChunks;

	unsigned int TicksCount = 0;
	std::thread NodeThread;
	bool stop = false;
};
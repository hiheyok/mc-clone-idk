#pragma once
#include "DimensionNode.h"
#include "../Chunk/ChunkMap.h"
class DimensionNodeContainer{
public:
	DimensionNodeContainer(DimensionProperties* Properties) : DProperties(Properties) {
		
	}

	void NewNode() {
		DNodes[nextDimensionID] = new DimensionNode;
		DNodes[nextDimensionID]->Initialize(DProperties, &DNodes, &ChunkDNodeLocation, nextDimensionID);
		DNodes[nextDimensionID]->Run();
		nextDimensionID++;
	}

	void AddChunk(Chunk& chunk) {
		Chunks[chunk.ChunkID] = chunk;
	}

	Chunk& getChunk(CHUNK_ID ChunkID) {
		return Chunks[ChunkID];
	}

	Chunk& operator[](CHUNK_ID ChunkID) {
		return Chunks[ChunkID];
	}

	void Stop() {
		for (const auto& node : DNodes) {
			node.second->Stop();
			ID id = node.second->NodeID;
			delete node.second;
			DNodes.erase(id);
		}
	}
private:
	std::unordered_map<ID, DimensionNode*> DNodes;
	std::unordered_map<CHUNK_ID, ID> ChunkDNodeLocation;

	ChunkMap Chunks;

	DimensionProperties* DProperties;
	ID nextDimensionID = NULL;
};
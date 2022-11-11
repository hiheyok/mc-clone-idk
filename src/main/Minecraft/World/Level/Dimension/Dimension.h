#pragma once
#include "DimensionProperties.h"
#include "DimensionNode.h"
#include "../Chunk/ChunkMap.h"
class Dimension : public DimensionProperties {
	Dimension() {
		Build();
	}
	virtual void Build() {

	}
	void Start() {

	}
	void NewNode() {
		DNodes[nextID] = new DimensionNode(this, &DNodes, &ChunkDNodeLocation);
		nextID++;
	}
private:
	std::unordered_map<ID, DimensionNode*> DNodes;
	std::unordered_map<CHUNK_ID, ID> ChunkDNodeLocation;
	ID nextID = NULL;
};
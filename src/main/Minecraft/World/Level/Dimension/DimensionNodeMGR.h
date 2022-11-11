#pragma once
#include "DimensionNode.h"
class DimensionNodeContainer {
public:
	DimensionNodeContainer(DimensionProperties* Properties) : DProperties(Properties) {

	}
	void NewNode() {
		DNodes[nextID] = new DimensionNode(DProperties, &DNodes, &ChunkDNodeLocation);
		nextID++;
	}
private:
	std::unordered_map<ID, DimensionNode*> DNodes;
	std::unordered_map<CHUNK_ID, ID> ChunkDNodeLocation;
	DimensionProperties* DProperties;
	ID nextID = NULL;
};
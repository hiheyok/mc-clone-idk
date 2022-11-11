#pragma once
#include "../Chunk/ChunkMap.h"
#include "DimensionProperties.h"

class DimensionNode : public ChunkMap {
public:
	DimensionNode(DimensionProperties* Properties, std::unordered_map<ID, DimensionNode*>* Nodes, std::unordered_map<CHUNK_ID, ID>* ChunkNodeLocation) : DProperties(Properties), DNodes(Nodes), ChunkDNodeLocation(ChunkNodeLocation) {};


private:
	DimensionProperties* DProperties;
	std::unordered_map<ID, DimensionNode*>* DNodes;
	std::unordered_map<CHUNK_ID, ID>* ChunkDNodeLocation;
};
#pragma once
#include "DimensionProperties.h"
#include "DimensionNodeMGR.h"
#include "../Chunk/ChunkMap.h"
class Dimension : public DimensionProperties {
public:
	Dimension() {
		Build();
	}
	virtual void Build() {

	}
	void Start() {
		Terrain = new DimensionNodeContainer(this);
	}

private:
	DimensionNodeContainer* Terrain;
	ID nextID = NULL;
};
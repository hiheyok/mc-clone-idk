#pragma once

#include "../Block.h"

class LeafBlock : public Block {
public:
	void Build() {
		friction = 1.0f;
		hasCollision = true;
		explosiveResistance = 0.5f;
		destroyTime = 0.5f;
		speedFactor = 1.0f;
		jumpFactor = 1.0f;
		hardness = 0.5f;
		tickable = true;
		transparency = true;
	}

	void Tick() {

	}
};
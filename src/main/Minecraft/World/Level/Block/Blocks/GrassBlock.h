#pragma once
#include "../Block.h"

class GrassBlock : public Block {
public:
	void Build() {
		friction = 1.0f;
		hasCollision = true;
		explosiveResistance = 1.0f;
		destroyTime = 1.0f;
		speedFactor = 1.0f;
		jumpFactor = 1.0f;
		hardness = 1.0f;
		tickable = true;
		transparency = false;
	}

	void Tick() {

	}
};
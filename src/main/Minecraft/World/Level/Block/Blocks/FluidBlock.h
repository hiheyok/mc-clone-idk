#pragma once

#include "../Block.h"

class FluidBlock : public Block {
public:
	void Build() {
		friction = 1.0f;
		hasCollision = false;
		explosiveResistance = 999999999.0f;
		destroyTime = 999999999.0f;
		speedFactor = 1.0f;
		jumpFactor = 1.0f;
		hardness = 999999999.0f;
		tickable = true;
		transparency = true;
	}

	void Tick() {

	}
};
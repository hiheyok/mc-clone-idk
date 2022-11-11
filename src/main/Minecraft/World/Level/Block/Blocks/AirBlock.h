#pragma once
#include "../Block.h"

class AirBlock : public Block {
public:
	void Build() {
		friction = NULL;
		hasCollision = false;
		explosiveResistance = NULL;
		destroyTime = NULL;
		speedFactor = NULL;
		jumpFactor = NULL;
		hardness = NULL;
		tickable = false;
		transparency = true;
	}

	void Tick() {

	}
};
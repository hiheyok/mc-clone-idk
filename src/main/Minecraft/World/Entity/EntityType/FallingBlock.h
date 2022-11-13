#pragma once
#include "../Entity.h"

class FallingBlock : public Entity {
public:
	void Build() {
		AABB.SetSize(1, 1, 1);
	}
private:
	BlockID BlockType = AIR;
};
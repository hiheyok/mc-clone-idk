#pragma once
#include "../Entity.h"

class Player : public Entity {
	void Build() {
		AABB.SetSize(3,3,3);
	}
};
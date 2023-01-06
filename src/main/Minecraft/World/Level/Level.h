#pragma once
#include "../../../Utils/Math/Vector/vec3.h"
#include "../../Core/Registry.h"
class Level {
public:
	void SetEntityVelocity(fvec3 Velocity) {
	

	}

private:
	std::unordered_map<UUID, Entity*> LevelEntityList;

};
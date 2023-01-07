#pragma once
#include "../../../Utils/Math/Vector/vec3.h"
#include "../../Core/Registry.h"
class Level {
public:
	virtual void Build() {

	}

protected:
	std::unordered_map<UUID, Entity> LevelEntityList;
	std::unordered_map<ID, Dimension*> DimensionList;

};
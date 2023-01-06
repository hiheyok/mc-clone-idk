#pragma once
#include "WorldProperties.h"
#include "Level/Level.h"
class World : public WorldProperties {
public:
	void Create() {

	}
private:
	Level* level;
};
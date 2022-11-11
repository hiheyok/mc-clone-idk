#pragma once
#include "Properties/Properties.h"

class BlockBehavior : public BlockProperty {
public:
	void copy(BlockProperty& properties) {
		friction = properties.friction;

	}
private:

};
#pragma once
#include "../AABB/AABB.h"
#include "EntityProperties.h"

typedef unsigned long long int UUID;

class Entity : public EntityProperties {
public:
	
	Entity() {
		Build();
	}

	virtual void Build() {

	}

	virtual void Tick() {

	}

	template <class T, class Y, class U> void SetPosition(T x, Y y, U z) {
		Position.x = (float)x;
		Position.y = (float)y;
		Position.z = (float)z;
		AABB.SetPos(x,y,z);
	}

	template <class T, class Y, class U > void Move(T x, Y y, U z) {
		Position.x += (float)x;
		Position.y += (float)y;
		Position.z += (float)z;
		AABB.SetPos(Position.x, Position.y, Position.z);
	}

	AABB_BOX AABB;

	UUID EntityUUID = 0;

	ID EntityType = NULL;

};


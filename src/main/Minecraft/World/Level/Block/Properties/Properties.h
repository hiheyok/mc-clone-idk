#pragma once

class BlockProperty {
public:
	float friction;
	bool hasCollision;
	float explosiveResistance;
	float destroyTime;
	float speedFactor;
	float jumpFactor;
	float hardness;

	bool tickable;
	bool transparency;

};
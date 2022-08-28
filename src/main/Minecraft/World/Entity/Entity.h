#pragma once

class Entity {
public:
	
	virtual void Build() {

	}

	virtual void Tick() {

	}

	template <typename T> void SetPosition(T x, T y, T z) {
		PosX = x;
		PosY = y;
		PosZ = z;
	}

	double PosX, PosY, PosZ = 0.0;
	double RotX, RotY, RotZ = 0.0;

	long long int EntityID = 0;
};


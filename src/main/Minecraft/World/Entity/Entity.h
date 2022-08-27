#pragma once

class Entity {
public:
	
	virtual void Build() {

	}

	virtual void Tick() {

	}

	double PosX, PosY, PosZ = 0.0;
	double RotX, RotY, RotZ = 0.0;

private:

	long long int EntityID = 0;
};


#pragma once
#include "../AABB/AABB.h"

class Entity {
public:
	
	Entity() {
		Build();
	}

	virtual void Build() {

	}

	virtual void Tick() {

	}

	template <class T, class Y, class U> void SetPosition(T x, Y y, U z) {
		PosX = (double)x;
		PosY = (double)y;
		PosZ = (double)z;
		AABB.SetPos(x,y,z);
	}

	template <class T, class Y, class U > void Move(T x, Y y, U z) {
		PosX += (double)x;
		PosY += (double)y;
		PosZ += (double)z;
		AABB.SetPos(PosX, PosY, PosZ);
	}

	double PosX, PosY, PosZ = 0.0;
	double RotX, RotY, RotZ = 0.0;

	double AccerlationX, AccerlationY, AccerlationZ = 0.0;
	double VelocityX, VelocityY, VelocityZ = 0.0;

	double Speed = 25.0;
	double Sink = 25.0;
	double Lift = 25.0;

	bool Jump = false;

	AABB_BOX AABB;

	long long int EntityID = 0;
};


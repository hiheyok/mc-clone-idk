#pragma once
#include "../../../Utils/Math/Vector/fvec3.h"

class EntityProperties {
public:
	EntityProperties() {
		DeltaRotation = fvec3(0, 0, 0);
		Rotation = fvec3(0, 0, 0);
		Accerlation = fvec3(0, 0, 0);
		Velocity = fvec3(0, 0, 0);
		Position = fvec3(0, 0, 0);
	}

	bool Jump = false;

	double Speed = 0.0;
	double Sink = 0.0;
	double Lift = 0.0;

	fvec3 DeltaRotation;
	fvec3 Rotation;
	fvec3 Accerlation;
	fvec3 Velocity;
	fvec3 Position;
};
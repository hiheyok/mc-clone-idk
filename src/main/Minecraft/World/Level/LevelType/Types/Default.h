#pragma once
#include "../../Level.h"
#include "../../Dimension/Dimensions.h"
class Default : public Level {
public:
	void Build() {
		DimensionList[OVERWORLD] = DimensionRegistry[OVERWORLD];
	}
};
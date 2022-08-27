#pragma once

#include "World/World.h"

class MinePlusPLus {
public:
	void Init() {
		TestWorld = new World;
		TestWorld->Initialize();


	}
private:
	World* TestWorld = nullptr;
};
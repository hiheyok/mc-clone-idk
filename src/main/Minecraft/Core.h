#pragma once
#define _CRTDBG_MAP_ALLOC
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
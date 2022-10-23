#pragma once
#include "WorldContainer.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
class World : public WorldContainer {
public:

	const unsigned int TPS = 20;

	void Initialize();

	void StartGenThreads(int amount);

	void WorldLoop();

	bool stop = false;

private:

	void SendChunkDataToClient();

	const int SpawnChunksSize = 8;
	std::thread WorldMainThread;
};
#ifndef CHUNK_CONTAINERH
#define CHUNK_CONTAINERH
#include "../../Utils/MutithreadedData.h"
#include "../Client/ClientWorld/ClientWorld.h"
#include "Entity/Entity.h"
#include <vector>
#include <algorithm>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <deque>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

class WorldContainer {
public:

    /*void Initialize();*/

    

    Chunk RequestChunk(int x, int y, int z);

    void JoinWorld(std::string PlayerName, ClientWorld* PlayerAddress);
    void LeaveWorld();

    AsyncHashMap<CHUNK_ID, Chunk> ChunkMapLoaded;
protected:

    void LoadChunk(CHUNK_ID ChunkID);

    void WriteChunkMapStore(Chunk& chunk);
    void WriteChunkMapLoad(Chunk& chunk);

    Chunk& ReadChunkMapStore(int x, int y, int z);
    Chunk& ReadChunkMapLoaded(int x, int y, int z);

    Chunk& ReadChunkMapStore(CHUNK_ID id);
    Chunk& ReadChunkMapLoaded(CHUNK_ID id);

    void UnloadChunk(int x, int y, int z);

    bool CheckChunkIsStored(int x, int y, int z);
    bool CheckChunkIsLoaded(int x, int y, int z);

    void AddWorldGenWorker();
    void DeleteWorldGenWorker();

    void WorldStats();
    void Initialize();

    void DoQueuedTasks();
    void SaveWorld(); //dump loaded data to store map

    void AddEntity(Entity Entity);
    void UpdatePlayerPosition(int Player_ID, int x, int y, int z);

    concurrency::concurrent_priority_queue<CHUNK_ID> ChunkLoadQueue;
    std::thread WorldStatsThread;
    AsyncHashMap<long long int, std::string> PlayerList;
    AsyncHashMap<long long int, ClientWorld*> PlayerAddress;
    AsyncHashMap<long long int, Entity> EntityList;

    Concurrency::concurrent_unordered_map<long long int, Concurrency::concurrent_unordered_set<CHUNK_ID> > ClientChunkToUpdate;
private:

    void AddChunkToGenQueue(int x, int y, int z);
    void WorldGenerator();

    int TickingDistance = 64;
    int QUEUED_TASKS_RATE = 2500;

    AsyncHashMap<CHUNK_ID, Chunk> ChunkMapStore; //simulated file system 
    std::unordered_set<CHUNK_ID> ChunkProcessing;
    std::mutex mut;
    concurrency::concurrent_priority_queue<CHUNK_ID> ChunkGenQueue;

    std::deque<std::thread> WorldGenWorkers;

    

    FastNoiseLite noise;
};


#endif
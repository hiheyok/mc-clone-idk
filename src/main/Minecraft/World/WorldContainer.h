#ifndef CHUNK_CONTAINERH
#define CHUNK_CONTAINERH
#include "../../Utils/MutithreadedData.h"
#include "../Client/ClientWorld/ClientWorld.h"
#include "Entity/Entity.h"
#include <vector>
#include <algorithm>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <string>
#include <deque>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>



class WorldContainer {
public:

    /*void Initialize();*/

    

    Chunk RequestChunk(int x, int y, int z);

    void JoinWorld(std::string PlayerName, ClientWorld* PlayerAddress);
    void LeaveWorld();

    AsyncHashMapClass<CHUNK_ID, Chunk> ChunkMapLoaded;
protected:

    void LoadChunk(int x, int y, int z);

    void WriteChunkMapStore(Chunk chunk);
    Chunk ReadChunkMapStore(int x, int y, int z);
    Chunk ReadChunkMapLoaded(int x, int y, int z);

    Chunk ReadChunkMapStore(long long int id);
    Chunk ReadChunkMapLoaded(long long int id);

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

    AsyncDeque<glm::ivec3> ChunkLoadQueue;
    std::thread WorldStatsThread;
    AsyncHashMapNonClass<long long int, std::string> PlayerList;
    AsyncHashMapNonClass<long long int, ClientWorld*> PlayerAddress;
    AsyncHashMapClass<long long int, Entity> EntityList;

    Concurrency::concurrent_unordered_map<long long int, Concurrency::concurrent_unordered_set<long long int> > ClientChunkToUpdate;
private:
    void WorldGenerator();

    int TickingDistance = 32;

    AsyncHashMapClass<CHUNK_ID, Chunk> ChunkMapStore;
    AsyncHashMapNonClass<CHUNK_ID, bool> ChunkProcessing;

    AsyncDeque<glm::ivec3> ChunkGenQueue;

    std::deque<std::thread> WorldGenWorkers;

    

    FastNoiseLite noise;
};


#endif
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

    void UnloadChunk(int x, int y, int z);

    bool CheckChunkIsStored(int x, int y, int z);
    bool CheckChunkIsLoaded(int x, int y, int z);

    void AddWorldGenWorker();
    void DeleteWorldGenWorker();

    void DoQueuedTasks();
    void SaveWorld(); //dump loaded data to store map

    void AddEntity(Entity Entity);
    void UpdatePlayerPosition(int Player_ID, int x, int y, int z);

    AsyncDeque<glm::ivec3> ChunkLoadQueue;

    AsyncHashMapNonClass<long long int, std::string> PlayerList;
    AsyncHashMapNonClass<long long int, ClientWorld*> PlayerAddress;
    AsyncHashMapClass<long long int, Entity> EntityList;

    AsyncHashMapClass<long long int, Chunk> ClientChunkToUpdate;
private:
    void WorldGenerator();

    int TickingDistance = 10;

    AsyncHashMapClass<CHUNK_ID, Chunk> ChunkMapStore;
    AsyncHashMapNonClass<CHUNK_ID, bool> ChunkProcessing;

    AsyncDeque<glm::ivec3> ChunkGenQueue;

    std::deque<std::thread> WorldGenWorkers;

    FastNoiseLite noise;
};


#endif
#ifndef CHUNK_CONTAINERH
#define CHUNK_CONTAINERH
#include "Level/Chunk/Chunk.h"
#include "../../utils/FastNoiseLite.h"
#include "../../utils/MathHelper.h"
#include "../../utils/Clock.h"
#include "../../utils/LogUtils.h"
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

    void LoadChunk(int x, int y, int z);

    void WriteChunkMapStore(Chunk chunk);
    Chunk ReadChunkMapStore(int x, int y, int z);

    void UnloadChunk(int x, int y, int z);

    bool CheckChunkIsStored(int x, int y, int z);
    bool CheckChunkIsLoaded(int x, int y, int z);

    Chunk RequestChunk(int x, int y, int z);

    void JoinWorld(std::string PlayerNmae, Entity* PlayerAddress);
    void LeaveWorld();

    std::unordered_map<CHUNK_ID, Chunk> ChunkMapLoaded;
protected:

    void AddWorldGenWorker();
    void DeleteWorldGenWorker();

    void DoQueuedTasks();
    void SaveWorld(); //dump loaded data to store map

    void AddEntity(Entity* Entity);

    std::deque<glm::ivec3> ChunkLoadQueue;

    std::unordered_map<long long int, std::string> PlayerList;
    std::unordered_map<long long int, Entity*> EntityList; 
private:
    void WorldGenerator();

    //bool GenQueueBusy = false;
    //bool ChunkStorageBusy = false;

    std::mutex ChunkStorageMutex;
    std::mutex ChunkGenQueueMutex;
    
    std::unordered_map<CHUNK_ID, Chunk> ChunkMapStore;
    std::unordered_map<CHUNK_ID, bool> ChunkProcessing;

    std::deque<glm::vec3> ChunkGenQueue;

    std::vector<std::thread> WorldGenWorkers;

    FastNoiseLite noise;
};


#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "ClientWorld.h"
#include "../../../Utils/Clock.h"
#include "../../../Utils/LogUtils.h"
#include "../../../Utils/MathHelper.h"

void ClientWorld::AddChunkServer(Chunk chunk) {
	auto t0 = std::chrono::high_resolution_clock::now();
	ChunkAddQueue.push(chunk);
//	CPU_TIME += (std::chrono::high_resolution_clock::now() - t0).count();
	
}

void ClientWorld::Start(GLFWwindow* window_) {
	ClientWorldThread = std::thread(&ClientWorld::ClientWorldMainLoop, this);
	player = new Player();
	player->Build();
	player->PosY = 128;
	TerrrainRenderer = new ChunkRenderer;
	TerrrainRenderer->init(window_, &camera);
}

void ClientWorld::UpdateChunks() {
	while (!ChunkAddQueue.empty()) {
		auto t0 = std::chrono::high_resolution_clock::now();
		Chunk chunk;
		if (ChunkAddQueue.try_pop(chunk)) {

			int x = chunk.pos.x;
			int y = chunk.pos.y;
			int z = chunk.pos.z;

			CHUNK_ID ChunkID = getChunkID(x, y, z);
			ChunkCache[ChunkID] = chunk; // Copy Chunk To Loaded Cache

			ChunkCache[ChunkID].clearNeighbors();
			if (ChunkCache.count(getChunkID(x, y + 1, z))) {
				ChunkCache[(x, y + 1, z)].setNeighborNY(&ChunkCache[ChunkID]);
				ChunkCache[ChunkID].setNeighborPY(&ChunkCache[getChunkID(x, y + 1, z)]);
				ChunkMeshQueue.push(getChunkID(x, y + 1, z));
			}
			if (ChunkCache.count(getChunkID(x, y - 1, z))) {
				ChunkCache[getChunkID(x, y - 1, z)].setNeighborPY(&ChunkCache[ChunkID]);
				ChunkCache[ChunkID].setNeighborNY(&ChunkCache[getChunkID(x, y - 1, z)]);
				ChunkMeshQueue.push(getChunkID(x, y - 1, z));
			}
			if (ChunkCache.count(getChunkID(x + 1, y, z))) {
				ChunkCache[getChunkID(x + 1, y, z)].setNeighborNX(&ChunkCache[ChunkID]);
				ChunkCache[ChunkID].setNeighborPX(&ChunkCache[getChunkID(x + 1, y, z)]);
				ChunkMeshQueue.push(getChunkID(x + 1, y, z));
			}
			if (ChunkCache.count(getChunkID(x - 1, y, z))) {
				ChunkCache[getChunkID(x - 1, y, z)].setNeighborPX(&ChunkCache[ChunkID]);
				ChunkCache[ChunkID].setNeighborNX(&ChunkCache[getChunkID(x - 1, y, z)]);
				ChunkMeshQueue.push(getChunkID(x - 1, y, z));
			}
			if (ChunkCache.count(getChunkID(x, y, z + 1))) {
				ChunkCache[getChunkID(x, y, z + 1)].setNeighborNZ(&ChunkCache[ChunkID]);
				ChunkCache[ChunkID].setNeighborPZ(&ChunkCache[getChunkID(x, y, z + 1)]);
				ChunkMeshQueue.push(getChunkID(x, y, z + 1));
			}
			if (ChunkCache.count(getChunkID(x, y, z - 1))) {
				ChunkCache[getChunkID(x, y, z - 1)].setNeighborPZ(&ChunkCache[ChunkID]);
				ChunkCache[ChunkID].setNeighborNZ(&ChunkCache[getChunkID(x, y, z - 1)]);
				ChunkMeshQueue.push(getChunkID(x, y, z - 1));
			}
			ChunkMeshQueue.push(getChunkID(x, y, z));
		}
	//	CPU_TIME += (std::chrono::high_resolution_clock::now() - t0).count();
	}
}

void ClientWorld::UpdatePlayer(double delta, std::unordered_map<char, bool> KeysInputs, glm::vec2 MouseMovement) {

	//lta = 1.0;
	double rad = 0.0174533; //deg to rad

	if (delta > 0.5) {
		delta = 0.5;
	}

	//Process Rotation
	MouseMovement.x *= (float)MouseSens;
	MouseMovement.y *= -(float)MouseSens;

	player->RotY += MouseMovement.x;
	player->RotZ += MouseMovement.y;

	if (player->RotZ > 89.99f)
		player->RotZ = 89.99f;
	if (player->RotZ < -89.99f)
		player->RotZ = -89.99f;

	if (player->RotY < 0.0f)
		player->RotY += 360.0f;
	if (player->RotY > 360.0f)
		player->RotY -= 360.0f;
	
	double accerlation = 35.0;
	double friction = 3.0;
	double Distance = 15 * delta;

	if (!TestIfEntityOnGround(player)) {
		Distance *= 0.5;
	}

	player->VelocityY -= accerlation * delta;
	if (TestIfEntityOnGround(player)) {
		player->VelocityX = player->VelocityX - ((player->VelocityX * delta) * (friction));
		//player->VelocityY = player->VelocityY - ((player->VelocityY * delta) * (1 - friction));
		player->VelocityZ = player->VelocityZ - ((player->VelocityZ * delta) * (friction));
		if (player->VelocityY < -2.5) {
			player->VelocityY = -(player->VelocityY) * 0.30;
		}
		else {
			player->VelocityY = 0;
			
		}
		
	}
	else {
		player->VelocityX = player->VelocityX - ((player->VelocityX * delta) * (friction * 0.35));
		player->VelocityY = player->VelocityY - ((player->VelocityY * delta) * (friction * 0.175));
		player->VelocityZ = player->VelocityZ - ((player->VelocityZ * delta) * (friction * 0.35));
	}

	if (player->VelocityX < 0) {
		if (TestIfEntityTouchBlockXN(player)) {
			player->VelocityX = player->VelocityX - ((player->VelocityX * delta) * (friction * 5));
		}
	}

	if (player->VelocityZ < 0) {
		if (TestIfEntityTouchBlockZN(player)) {
			player->VelocityZ = player->VelocityZ - ((player->VelocityZ * delta) * (friction * 5));
		}
	}

	if (player->VelocityX > 0) {
		if (TestIfEntityTouchBlockXP(player)) {
			player->VelocityX = player->VelocityX - ((player->VelocityX * delta) * (friction * 5));
		}
	}

	if (player->VelocityZ > 0) {
		if (TestIfEntityTouchBlockZP(player)) {
			player->VelocityZ = player->VelocityZ - ((player->VelocityZ * delta) * (friction * 5));
		}
	}

	if (player->VelocityY > 0) {
		if (TestIfEntityTouchBlockYP(player)) {
			player->VelocityY = player->VelocityY - ((player->VelocityY * delta) * (friction * accerlation));
		}
	}


	//Process movemnet
	if (KeysInputs.count('R') || KeysInputs.count('r')) {
		player->SetPosition(0,128,0);
		player->RotY = 20;
		player->RotZ = 0;
	}
	if (KeysInputs.count('W') || KeysInputs.count('w')) {
		player->VelocityX += Distance * cos(rad * player->RotY);
		player->VelocityZ += Distance * sin(rad * player->RotY);
	}
	if (KeysInputs.count('S') || KeysInputs.count('s')) {
		player->VelocityX += -Distance * cos(rad * player->RotY);
		player->VelocityZ += -Distance * sin(rad * player->RotY);
	}
	if (KeysInputs.count('A') || KeysInputs.count('a')) {
		player->VelocityX += -Distance * cos(rad * (player->RotY + 90));
		player->VelocityZ += -Distance * sin(rad * (player->RotY + 90));
	}
	if (KeysInputs.count('D') || KeysInputs.count('d')) {
		player->VelocityX += Distance * cos(rad * (player->RotY + 90));
		player->VelocityZ += Distance * sin(rad * (player->RotY + 90));
	}

	if (KeysInputs.count('F') || KeysInputs.count('f')) {
		int sx = 0, sy = 0, sz = 0;

		printf("Velocity (x,y,z): ");

		scanf("%d %d %d", &sx, &sy, &sz);

		player->VelocityX = sx;
		player->VelocityY = sy;
		player->VelocityZ = sz;
	}
	
	if (KeysInputs.count(' ')) {
		if (TestIfEntityOnGround(player)) {
			player->VelocityY = 10.0;//movey += Distance;
		}
		
	}
	if (KeysInputs.count(0)) {
		player->VelocityY = 0;
	}

	if (KeysInputs.count('G') || KeysInputs.count('g')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	

	double movex = player->VelocityX * delta;
	double movey = player->VelocityY * delta;
	double movez = player->VelocityZ * delta;


	MoveEntity(player, movex, movey, movez);
//	MoveEntity(player, 0, -Gravity * delta, 0);
}

glm::vec3 ClientWorld::GetPlayerPos() {
	return glm::vec3(player->PosX, player->PosY, player->PosZ);
}

glm::vec3 ClientWorld::GetPlayerSpeed() {
	return glm::vec3(player->VelocityX, player->VelocityY, player->VelocityZ);
}

void ClientWorld::MesherWorker() {
	ChunkMesh Mesher;
	
	Chunk PX, NX, PY, NY, PZ, NZ;

	Chunk chunk;
	long long int ChunkID = 0;
	while (!stop) {
		while (ChunkMeshQueue.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(12)); 
		}
		
		if (ChunkMeshQueue.size() != 0) {
			if (ChunkMeshQueue.try_pop(ChunkID)) {

				Mesher.chunk = &ChunkCache[ChunkID];
				if (!Mesher.chunk->isEmpty()) {
					auto t0 = std::chrono::high_resolution_clock::now();
					Mesher.SmartGreedyMeshing();
					
					ChunkVerticesData MeshData;
					MeshData.x = Mesher.chunk->pos.x;
					MeshData.y = Mesher.chunk->pos.y;
					MeshData.z = Mesher.chunk->pos.z;
					MeshData.SolidVertices = Mesher.vertices;
					MeshData.TransparentVertices = Mesher.transparentVertices;
					Mesher.delete_();
					
					TerrrainRenderer->AddChunkQueue(MeshData);
					CPU_TIME += (std::chrono::high_resolution_clock::now() - t0).count();
				}
				
			}
		}
		
	}
}

void ClientWorld::AddMeshWorker() {
	MeshWorkers.push_back(std::thread(&ClientWorld::MesherWorker, this));
}


void ClientWorld::Render() {
	
	TerrrainRenderer->draw();
}

void ClientWorld::PrepareRenderer() {
	camera.Position = glm::vec3((float)player->PosX, (float)player->PosY, (float)player->PosZ);
	camera.Yaw = (float)player->RotY;
	camera.Pitch = (float)player->RotZ;
	camera.updateCameraVectors();
	TerrrainRenderer->DumpQueuedDataToGPU();
	TerrrainRenderer->GenCallDrawCommands();
	TerrrainRenderer->UpdateData();
}

void ClientWorld::ClientWorldMainLoop() {
	while (!stop) {
		auto time0 = std::chrono::high_resolution_clock::now();
		
		//Process all chunks update / additions first
		UpdateChunks();

		Tick();

		double time1 = ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
		if (time1 < (1.0 / (double)TPS))
			timerSleepNotPrecise(((1.0 / (double)TPS) - time1)*1000);
	//	getLogger()->LogDebug("Client", std::to_string(CPU_TIME / 1000000) + " ms");
	}
}

void ClientWorld::Tick() {
	//MoveEntity(player, 0, -Gravity, 0);
}
double size = 1;
void ClientWorld::MoveEntity(Entity* ENTITY, double x, double y, double z) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	bool collusion_x = false;
	bool collusion_y = false;
	bool collusion_z = false;

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		/*getLogger()->LogDebug("Client World", "Chunk: " + std::to_string(cx) + ", " + std::to_string(cy) + ", " + std::to_string(cz));
		getLogger()->LogDebug("Client World", "Local Pos: " + std::to_string(lx) + ", " + std::to_string(ly) + ", " + std::to_string(lz));
		getLogger()->LogDebug("Client World", "Global Pos: " + std::to_string(ENTITY->PosX) + ", " + std::to_string(ENTITY->PosY) + ", " + std::to_string(ENTITY->PosZ));*/
		Chunk CHUNK = ChunkCache[getChunkID(cx,cy,cz)];

		

		if (x > 0) {
			if (CHUNK.checkblock(FastFloor(lx + x + size), ly, lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx + 1 - x, ly, lz)) {
					collusion_x = true;
			//	}
			}
		}
		if (x < 0) {
			if (CHUNK.checkblock(FastFloor(lx -x - size), ly, lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx - 1 + x, ly, lz)) {
					collusion_x = true;
			//	}
			}
		}

		if (y > 0) {
			if (CHUNK.checkblock(lx, FastFloor(ly + y + size + 1), lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly + 1 - y, lz)) {
					collusion_y = true;
			//	}
			}
		}
		if (y < 0) {
			if (CHUNK.checkblock(lx, FastFloor(ly - y - size - 1), lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly - 1 + y, lz)) {
					collusion_y = true;
			//	}
			}
		}

		if (z > 0) {
			if (CHUNK.checkblock(lx, ly, FastFloor(lz + z + size)).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly, lz + 1 - z)) {
					collusion_z = true;
			//	}
			}
		}
		if (z < 0) {
			if (CHUNK.checkblock(lx, ly, FastFloor(lz - z - size)).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly, lz - 1 + z)) {
					collusion_z = true;
			//	}
			}
		}
	}
	if (!collusion_x) {
		ENTITY->Move(x, 0, 0);
	}
	if (!collusion_y) {
		ENTITY->Move(0, y, 0);
	}
	if (!collusion_z) {
		ENTITY->Move(0, 0, z);
	}
}

bool ClientWorld::TestIfEntityOnGround(Entity* ENTITY) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache[getChunkID(cx, cy, cz)];
		if (CHUNK.checkblock(lx, FastFloor(ly - size -1), lz).id != AIR) {
			return true;
		}
	}
	return false;
}

bool ClientWorld::TestIfEntityTouchBlockYP(Entity* ENTITY) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache[getChunkID(cx, cy, cz)];
		if (CHUNK.checkblock(lx, FastFloor(ly + size + 1), lz).id != AIR) {
			return true;
		}
	}
	return false;
}


bool ClientWorld::TestIfEntityTouchBlockXP(Entity* ENTITY) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache[getChunkID(cx, cy, cz)];
		if ((CHUNK.checkblock(FastFloor(lx + size), ly, lz).id != AIR)) {
			return true;
		}

	}
	return false;
}

bool ClientWorld::TestIfEntityTouchBlockZP(Entity* ENTITY) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache[getChunkID(cx, cy, cz)];
		if ((CHUNK.checkblock(lx, ly, FastFloor(lz + size)).id != AIR)) {
			return true;
		}
	}
	return false;
}


bool ClientWorld::TestIfEntityTouchBlockXN(Entity* ENTITY) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache[getChunkID(cx, cy, cz)];
		if ((CHUNK.checkblock(FastFloor(lx - size), ly, lz).id != AIR)) {
			return true;
		}

	}
	return false;
}

bool ClientWorld::TestIfEntityTouchBlockZN(Entity* ENTITY) {
	int cx = FastFloor(ENTITY->PosX * 0.0625);
	int cy = FastFloor(ENTITY->PosY * 0.0625);
	int cz = FastFloor(ENTITY->PosZ * 0.0625);

	int lx = FastFloor(ENTITY->PosX - (FastFloor(ENTITY->PosX * 0.0625) * 16));
	int ly = FastFloor(ENTITY->PosY - (FastFloor(ENTITY->PosY * 0.0625) * 16));
	int lz = FastFloor(ENTITY->PosZ - (FastFloor(ENTITY->PosZ * 0.0625) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache[getChunkID(cx, cy, cz)];
		if ((CHUNK.checkblock(lx, ly, FastFloor(lz - size)).id != AIR)) {
			return true;
		}
	}
	return false;
}


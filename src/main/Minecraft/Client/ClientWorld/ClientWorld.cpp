#include "ClientWorld.h"
#include "../../../Utils/Clock.h"
#include "../../../Utils/LogUtils.h"
#include "../../../Utils/MathHelper.h"

void ClientWorld::AddChunkServer(Chunk chunk) {
	ChunkAddQueue.push_back(chunk);
	
	
}

void ClientWorld::Start(GLFWwindow* window_) {
	ClientWorldThread = std::thread(&ClientWorld::ClientWorldMainLoop, this);
	player = new Player();
	player->Build();
	player->PosY = 500;
	TerrrainRenderer = new ChunkRenderer;
	TerrrainRenderer->init(window_, &camera);
}

void ClientWorld::UpdateChunks() {
	while (!ChunkAddQueue.empty()) {
		
		Chunk chunk = ChunkAddQueue.pop_get_front();

		int x = chunk.pos.x;
		int y = chunk.pos.y;
		int z = chunk.pos.z;

		CHUNK_ID ChunkID = getChunkID(x, y, z);
		ChunkCache.insert(ChunkID, chunk); // Copy Chunk To Loaded Cache
		
		ChunkCache.RunObjFunction(ChunkID, &Chunk::clearNeighbors);
		if (ChunkCache.count(getChunkID(x, y + 1, z))) {
			ChunkCache.RunObjFunction(getChunkID(x, y + 1, z), &Chunk::setNeighborNY, ChunkCache.getAddress(ChunkID));
			ChunkCache.RunObjFunction(ChunkID, &Chunk::setNeighborPY, ChunkCache.getAddress(getChunkID(x, y + 1, z)));
			AddChunkToRenderQueue(ChunkCache.get(getChunkID(x, y + 1, z)));
		}
		if (ChunkCache.count(getChunkID(x, y - 1, z))) {
			ChunkCache.RunObjFunction(getChunkID(x, y - 1, z), &Chunk::setNeighborPY, ChunkCache.getAddress(ChunkID));
			ChunkCache.RunObjFunction(ChunkID, &Chunk::setNeighborNY, ChunkCache.getAddress(getChunkID(x, y - 1, z)));
			AddChunkToRenderQueue(ChunkCache.get(getChunkID(x, y - 1, z)));
		}
		if (ChunkCache.count(getChunkID(x + 1, y, z))) {
			ChunkCache.RunObjFunction(getChunkID(x + 1, y, z), &Chunk::setNeighborNX, ChunkCache.getAddress(ChunkID));
			ChunkCache.RunObjFunction(ChunkID, &Chunk::setNeighborPX, ChunkCache.getAddress(getChunkID(x + 1, y, z)));
			AddChunkToRenderQueue(ChunkCache.get(getChunkID(x + 1, y, z)));
		}
		if (ChunkCache.count(getChunkID(x - 1, y, z))) {
			ChunkCache.RunObjFunction(getChunkID(x - 1, y, z), &Chunk::setNeighborPX, ChunkCache.getAddress(ChunkID));
			ChunkCache.RunObjFunction(ChunkID, &Chunk::setNeighborNX, ChunkCache.getAddress(getChunkID(x - 1, y, z)));
			AddChunkToRenderQueue(ChunkCache.get(getChunkID(x - 1, y, z)));
		}
		if (ChunkCache.count(getChunkID(x, y, z + 1))) {
			ChunkCache.RunObjFunction(getChunkID(x, y, z + 1), &Chunk::setNeighborNZ, ChunkCache.getAddress(ChunkID));
			ChunkCache.RunObjFunction(ChunkID, &Chunk::setNeighborPZ, ChunkCache.getAddress(getChunkID(x, y, z + 1)));
			AddChunkToRenderQueue(ChunkCache.get(getChunkID(x, y, z + 1)));
		}
		if (ChunkCache.count(getChunkID(x, y, z - 1))) {
			ChunkCache.RunObjFunction(getChunkID(x, y, z - 1), &Chunk::setNeighborPZ, ChunkCache.getAddress(ChunkID));
			ChunkCache.RunObjFunction(ChunkID, &Chunk::setNeighborNZ, ChunkCache.getAddress(getChunkID(x, y, z - 1)));
			AddChunkToRenderQueue(ChunkCache.get(getChunkID(x, y, z - 1)));
		}
		AddChunkToRenderQueue(chunk);
	}
}

void ClientWorld::AddChunkToRenderQueue(Chunk chunk) {

	chunk.clearNeighbors();

	int x = chunk.pos.x;
	int y = chunk.pos.y;
	int z = chunk.pos.z;

	CHUNK_ID ChunkID = getChunkID(x, y, z);
	RenderChunkUpdateQueue.insert(ChunkID, chunk); // Copy Chunk To Loaded Cache
	RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::clearNeighbors);
	if (RenderChunkUpdateQueue.count(getChunkID(x, y + 1, z))) {
		RenderChunkUpdateQueue.RunObjFunction(getChunkID(x, y + 1, z), &Chunk::setNeighborNY, RenderChunkUpdateQueue.getAddress(ChunkID));
		RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::setNeighborPY, RenderChunkUpdateQueue.getAddress(getChunkID(x, y + 1, z)));
	}
	if (RenderChunkUpdateQueue.count(getChunkID(x, y - 1, z))) {
		RenderChunkUpdateQueue.RunObjFunction(getChunkID(x, y - 1, z), &Chunk::setNeighborPY, RenderChunkUpdateQueue.getAddress(ChunkID));
		RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::setNeighborNY, RenderChunkUpdateQueue.getAddress(getChunkID(x, y - 1, z)));
	}
	if (RenderChunkUpdateQueue.count(getChunkID(x + 1, y, z))) {
		RenderChunkUpdateQueue.RunObjFunction(getChunkID(x + 1, y, z), &Chunk::setNeighborNX, RenderChunkUpdateQueue.getAddress(ChunkID));
		RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::setNeighborPX, RenderChunkUpdateQueue.getAddress(getChunkID(x + 1, y, z)));
	}
	if (RenderChunkUpdateQueue.count(getChunkID(x - 1, y, z))) {
		RenderChunkUpdateQueue.RunObjFunction(getChunkID(x - 1, y, z), &Chunk::setNeighborPX, RenderChunkUpdateQueue.getAddress(ChunkID));
		RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::setNeighborNX, RenderChunkUpdateQueue.getAddress(getChunkID(x - 1, y, z)));
	}
	if (RenderChunkUpdateQueue.count(getChunkID(x, y, z + 1))) {
		RenderChunkUpdateQueue.RunObjFunction(getChunkID(x, y, z + 1), &Chunk::setNeighborNZ, RenderChunkUpdateQueue.getAddress(ChunkID));
		RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::setNeighborPZ, RenderChunkUpdateQueue.getAddress(getChunkID(x, y, z + 1)));
	}
	if (RenderChunkUpdateQueue.count(getChunkID(x, y, z - 1))) {
		RenderChunkUpdateQueue.RunObjFunction(getChunkID(x, y, z - 1), &Chunk::setNeighborPZ, RenderChunkUpdateQueue.getAddress(ChunkID));
		RenderChunkUpdateQueue.RunObjFunction(ChunkID, &Chunk::setNeighborNZ, RenderChunkUpdateQueue.getAddress(getChunkID(x, y, z - 1)));
	}
}

void ClientWorld::UpdatePlayer(double delta, std::unordered_map<char, bool> KeysInputs, glm::vec2 MouseMovement) {

	
	double rad = 0.0174533; //deg to rad

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
	double friction = 2.0;
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
		player->VelocityX = player->VelocityX - ((player->VelocityX * delta) * (friction * 0.4));
		//player->VelocityY = player->VelocityY - ((player->VelocityY * delta) * (1 - friction));
		player->VelocityZ = player->VelocityZ - ((player->VelocityZ * delta) * (friction * 0.4));
	}


	//Process movemnet
	if (KeysInputs.count('R') || KeysInputs.count('r')) {
		player->SetPosition(0,500,0);
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
	
	if (KeysInputs.count(' ')) {
		if (TestIfEntityOnGround(player)) {
			player->VelocityY = 10.0;//movey += Distance;
		}
		
	}
	if (KeysInputs.count(0)) {
		player->VelocityY = 0;
	}
	
	

	double movex = player->VelocityX * delta;
	double movey = player->VelocityY * delta;
	double movez = player->VelocityZ * delta;


	MoveEntity(player, movex, movey, movez);
//	MoveEntity(player, 0, -Gravity * delta, 0);
}

void ClientWorld::DumpRenderQueuedData() {
	
	std::unordered_map<CHUNK_ID, Chunk> Map = RenderChunkUpdateQueue.DumpData();
	RenderChunkUpdateQueue.clear();
	
	for (auto& chunk : Map) {
		if (!chunk.second.isEmpty()) {
			ChunkMesh mesh;
			mesh.chunk = &chunk.second;
			mesh.SmartGreedyMeshing();
			ChunkVerticesData Vertices;
			Vertices.SolidVertices = mesh.vertices;
			Vertices.TransparentVertices = mesh.transparentVertices;
			Vertices.x = chunk.second.pos.x;
			Vertices.y = chunk.second.pos.y;
			Vertices.z = chunk.second.pos.z;
			TerrrainRenderer->AddChunkQueue(Vertices);
		}
	}

	Map.clear();
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

		//dump data to renderer
		DumpRenderQueuedData(); // switch to main thread later

		Tick();

		double time1 = ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
		if (time1 < (1.0 / (double)TPS))
			timerSleep((1.0 / (double)TPS) - time1);
	//	getLogger()->LogDebug("Client Tick", std::to_string(1000000000.0 / (std::chrono::high_resolution_clock::now() - time0).count()));
	}
}

void ClientWorld::Tick() {
	//MoveEntity(player, 0, -Gravity, 0);
}

void ClientWorld::MoveEntity(Entity* ENTITY, double x, double y, double z) {
	int cx = floor(ENTITY->PosX / 16.0);
	int cy = floor(ENTITY->PosY / 16.0);
	int cz = floor(ENTITY->PosZ / 16.0);

	double lx = floor(ENTITY->PosX - (double)(floor(ENTITY->PosX / 16.0) * 16));
	double ly = floor(ENTITY->PosY - (double)(floor(ENTITY->PosY / 16.0) * 16));
	double lz = floor(ENTITY->PosZ - (double)(floor(ENTITY->PosZ / 16.0) * 16));

	bool collusion_x = false;
	bool collusion_y = false;
	bool collusion_z = false;

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		/*getLogger()->LogDebug("Client World", "Chunk: " + std::to_string(cx) + ", " + std::to_string(cy) + ", " + std::to_string(cz));
		getLogger()->LogDebug("Client World", "Local Pos: " + std::to_string(lx) + ", " + std::to_string(ly) + ", " + std::to_string(lz));
		getLogger()->LogDebug("Client World", "Global Pos: " + std::to_string(ENTITY->PosX) + ", " + std::to_string(ENTITY->PosY) + ", " + std::to_string(ENTITY->PosZ));*/
		Chunk CHUNK = ChunkCache.get(getChunkID(cx,cy,cz));

		double size = 0.5;

		if (x > 0) {
			if (CHUNK.checkblock(floor(lx + x + size), ly, lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx + 1 - x, ly, lz)) {
					collusion_x = true;
			//	}
			}
		}
		if (x < 0) {
			if (CHUNK.checkblock(floor(lx -x - size), ly, lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx - 1 + x, ly, lz)) {
					collusion_x = true;
			//	}
			}
		}

		if (y > 0) {
			if (CHUNK.checkblock(lx, floor(ly + y + size), lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly + 1 - y, lz)) {
					collusion_y = true;
			//	}
			}
		}
		if (y < 0) {
			if (CHUNK.checkblock(lx, floor(ly - y - size), lz).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly - 1 + y, lz)) {
					collusion_y = true;
			//	}
			}
		}

		if (z > 0) {
			if (CHUNK.checkblock(lx, ly, floor(lz + z + size)).id != AIR) {
			//	if (ENTITY->AABB.TestIntersect(lx, ly, lz + 1 - z)) {
					collusion_z = true;
			//	}
			}
		}
		if (z < 0) {
			if (CHUNK.checkblock(lx, ly, floor(lz - z - size)).id != AIR) {
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
	int cx = floor(ENTITY->PosX / 16.0);
	int cy = floor(ENTITY->PosY / 16.0);
	int cz = floor(ENTITY->PosZ / 16.0);

	double lx = floor(ENTITY->PosX - (double)(floor(ENTITY->PosX / 16.0) * 16));
	double ly = floor(ENTITY->PosY - (double)(floor(ENTITY->PosY / 16.0) * 16));
	double lz = floor(ENTITY->PosZ - (double)(floor(ENTITY->PosZ / 16.0) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache.get(getChunkID(cx, cy, cz));
		if (CHUNK.checkblock(lx, floor(ly - 0.5), lz).id != AIR) {
			return true;
		}
	}
	return false;
}

bool ClientWorld::TestIfEntityTouchBlockX(Entity* ENTITY) {
	int cx = floor(ENTITY->PosX / 16.0);
	int cy = floor(ENTITY->PosY / 16.0);
	int cz = floor(ENTITY->PosZ / 16.0);

	double lx = floor(ENTITY->PosX - (double)(floor(ENTITY->PosX / 16.0) * 16));
	double ly = floor(ENTITY->PosY - (double)(floor(ENTITY->PosY / 16.0) * 16));
	double lz = floor(ENTITY->PosZ - (double)(floor(ENTITY->PosZ / 16.0) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache.get(getChunkID(cx, cy, cz));
		if ((CHUNK.checkblock(floor(lx - 0.5), ly, lz).id != AIR) || (CHUNK.checkblock(floor(lx + 0.5), ly, lz).id != AIR)) {
			return true;
		}

	}
	return false;
}

bool ClientWorld::TestIfEntityTouchBlockZ(Entity* ENTITY) {
	int cx = floor(ENTITY->PosX / 16.0);
	int cy = floor(ENTITY->PosY / 16.0);
	int cz = floor(ENTITY->PosZ / 16.0);

	double lx = floor(ENTITY->PosX - (double)(floor(ENTITY->PosX / 16.0) * 16));
	double ly = floor(ENTITY->PosY - (double)(floor(ENTITY->PosY / 16.0) * 16));
	double lz = floor(ENTITY->PosZ - (double)(floor(ENTITY->PosZ / 16.0) * 16));

	if (ChunkCache.count(getChunkID(cx, cy, cz))) {
		Chunk CHUNK = ChunkCache.get(getChunkID(cx, cy, cz));
		if ((CHUNK.checkblock(lx, ly, floor(lz - 0.5)).id != AIR) || (CHUNK.checkblock(lx, ly, floor(lz + 0.5)).id != AIR)) {
			return true;
		}
	}
	return false;
}


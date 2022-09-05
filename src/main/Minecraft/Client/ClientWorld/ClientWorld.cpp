#include "ClientWorld.h"
#include "../../../Utils/Clock.h"
#include "../../../Utils/LogUtils.h"
#include "../../../Utils/MathHelper.h"

void ClientWorld::AddChunkServer(Chunk chunk) {
	ChunkAddQueue.push_back(chunk);
}

void ClientWorld::Start(GLFWwindow* window_) {
	ClientWorldThread = std::thread(&ClientWorld::ClientWorldMainLoop, this);
	player = new Player;
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

void ClientWorld::UpdatePlayer(double delta, std::unordered_map<char, bool> KeysInputs) {

	double Distance = player->Speed * delta; //Distance travel per unit of time
	double rad = 0.0174533; //deg to rad

	//Process movemnet
	if (KeysInputs.count('W') || KeysInputs.count('w')) {
		player->PosX += Distance * cos(rad * player->RotY);
		player->PosZ += Distance * sin(rad * player->RotY);
	}
	if (KeysInputs.count('S') || KeysInputs.count('s')) {
		player->PosX -= Distance * cos(rad * player->RotY);
		player->PosZ -= Distance * sin(rad * player->RotY);
	}
	if (KeysInputs.count('D') || KeysInputs.count('d')) {
		player->PosX += Distance * sin(rad * player->RotY);
		player->PosZ += Distance * cos(rad * player->RotY);
	}
	if (KeysInputs.count('A') || KeysInputs.count('a')) {
		player->PosX -= Distance * sin(rad * player->RotY);
		player->PosZ -= Distance * cos(rad * player->RotY);
	}
	if (KeysInputs.count(' ')) {
		player->PosY += Distance;
	}

	//Process Rotation
	MouseMovement.x *= (float)MouseSens;
	MouseMovement.y *= (float)MouseSens;

	player->RotY += MouseMovement.x;
	player->RotZ += MouseMovement.y;

	if (player->RotZ > 89.99f)
		player->RotZ = 89.99f;
	if (player->RotZ < -89.99f)
		player->RotZ = -89.99f;

	if (player->RotY < 0.0f)
		player->RotY += 360.0f;
	if (player->RotY > 360.0f);
		player->RotY -= 360.0f;
}

void ClientWorld::DumpRenderQueuedData() {
	
	std::unordered_map<CHUNK_ID, Chunk> Map = RenderChunkUpdateQueue.DumpData();
	RenderChunkUpdateQueue.clear();
	
	for (auto chunk : Map) {
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
	TerrrainRenderer->UpdateData();
}

void ClientWorld::ClientWorldMainLoop() {
	while (!stop) {
		auto time0 = std::chrono::high_resolution_clock::now();
		
		//Process all chunks update / additions first
		UpdateChunks();

		//dump data to renderer
		DumpRenderQueuedData(); // switch to main thread later


		double time1 = ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
		if (time1 < (1.0 / (double)TPS))
			timerSleep((1.0 / (double)TPS) - time1);
		getLogger()->LogDebug("Client Tick", std::to_string(1000000000.0 / (std::chrono::high_resolution_clock::now() - time0).count()));
	}
}
#include "server.h"

void Server::start() {

}

void Server::startWorld() {
	World* _worldl = new World;
	world = _worldl;
	world->start(7);
}
void Server::processRequests() {

	int requestscount = 0;

	for (int i = 0; i < requestsQueued.size(); i++) {
		REQUEST req = requestsQueued[i];
		if (req.type == CHUNK) {
			if (world->checkChunk(req.x, req.y, req.z)) {
				requestscount++;
				PACKET pac;
				pac.type = CHUNK;
				pac.READY;
				Chunk* c = new Chunk;
				*c = world->returnChunkData(req.x, req.y, req.z);
				pac.chunk = c;
				//std::cout << "[ Info ]: Checking Chunk: " << pac.chunk->getcpos().x << ", " << pac.chunk->getcpos().y << ", " << pac.chunk->getcpos().z << "\n";
				req.ADDRESS->push_back(pac);
				requestsQueued.erase(requestsQueued.begin() + i);
			}
			else {
				world->loadChunks(req.x, req.y, req.z);
			}
		}
	}
	//std::cout << this << " | SERVER: Request Count: " << requestscount << "\n";
}
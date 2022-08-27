#ifndef SERVER_H
#define SERVER_H

#include "World/World.h"
#include "IEntity.h"
#include "../Client/entity/player/mainplayer.h"
#include <unordered_map>
#include <thread>
#include <deque>
#include "../utils/functions/clock/Clock.h"

enum SERVER_TYPE {
	INTEGRATED_SERVER
};

enum DATA_REQUEST_PACKET_TYPE {
	CHUNK
};

struct PACKET {
	bool READY = false;
	DATA_REQUEST_PACKET_TYPE type;
	Chunk* chunk;
};

struct REQUEST {
	DATA_REQUEST_PACKET_TYPE type;
	int x;
	int y;
	int z;
	std::vector<PACKET>* ADDRESS;
};



class Server {
public:
	Server(SERVER_TYPE _type) {
		type = _type;
		server_thread = new std::thread(&Server::run, this);
	}

	void run() {

		std::cout << "Integrated Server Thread : Thread ID: " << std::this_thread::get_id() << "\n";

		startWorld();

		for (int x = -10; x <= 10; x++) {
			for (int y = 0; y <= 10; y++) {
				for (int z = -10; z <= 10; z++) {
					world->loadChunks(x,y,z);
				}
			}
		}

		ServerLoop();
		
	}

	void ServerLoop() {
		
		while (true) {
			auto time0 = std::chrono::high_resolution_clock::now();
			if (requestsQueued.size() != 0) {
				processRequests();
			}

			

			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::chrono::duration<double, std::milli> looptime = std::chrono::high_resolution_clock::now() - time0;

			timerSleep(0.05 - (looptime.count() / 1000.0));
		}
	}

	void addEntity() {

	}


	void request(REQUEST req) {
		if (requestsQueued.size() < RequestQueueSizeLimit) {
			requestsQueued.emplace_back(req);
		}
	}

	const int RequestQueueSizeLimit = 4000;
	const int MaxRequestProcessPerSecond = 100;
	SERVER_TYPE type;
private:

	void processRequests();
	std::vector<REQUEST> requestsQueued;

	void start();

	void startWorld();

	std::unordered_map<std::string, Player> Players;

	std::thread* server_thread;

	

	World* world;

	const unsigned int TPS = 20;
};

#endif
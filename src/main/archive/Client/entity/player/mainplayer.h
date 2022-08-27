#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "../IEntity.h"

class Player : public IEntity {


public:

	Player() {
		player.EntityID = PLAYER;
	}

	//World* world;

private:
	//Camera* camera;
	IEntity player;
};

#endif
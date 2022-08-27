#ifndef IENTITY_H
#define IENTITY_H

#define PLAYER 0x00;

#include "../../utils/functions/vector/vectors.h"
#include "../../Server/world/World.h"

enum ENTITY_TYPE {
	PASSIVE, HOSTILE
};

class IEntity {

public:

	p_ivec3 pos = p_ivec3(0, 0, 0);
	p_ivec3 cpos = p_ivec3(0,0,0);
	Chunk* chunk = nullptr;
	char EntityID = 0;

protected:

	bool collision = false;



private:


};

#endif
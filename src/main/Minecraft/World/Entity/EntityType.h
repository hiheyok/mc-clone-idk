#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <unordered_map>
#include <string>

#include "Entity.h"
#include "Entities/Player.h"


class EntityType {
public:
	Entity* PLAYER = Register("mineplusplus:player", new Player);

private:
	Entity* Register(std::string NAME, Entity* ENTITY);
};
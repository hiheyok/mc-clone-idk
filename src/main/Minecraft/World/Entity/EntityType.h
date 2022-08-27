#pragma once

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
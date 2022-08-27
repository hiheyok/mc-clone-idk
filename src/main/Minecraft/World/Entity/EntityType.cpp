#include "EntityType.h"

Entity* EntityType::Register(std::string NAME, Entity* ENTITY) {

	ENTITY->Build();

	return ENTITY;

	//register later
}
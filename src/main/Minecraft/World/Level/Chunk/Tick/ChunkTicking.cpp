#include "../Chunk.h"
#include "../../TickSync/TickSync.h"
void Chunk::EntityTick() {
	for (const auto ENTITY : EntityList) {
		fvec3 Velocity = ENTITY.second->Velocity;
		fvec3 Movement;

		ENTITY.second->Position += Velocity;
		ENTITY.second->Velocity *= 0.25;
	}

	for (const auto ENTITY : QueuedEntityUpdates) {
		EntityList[ENTITY.second->EntityUUID] = ENTITY.second;
	}

	QueuedEntityUpdates.clear();
}

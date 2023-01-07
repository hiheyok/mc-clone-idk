#pragma once

#include "World.h"
#include <string>
#include "../Client/Settings/ClientSettings.h"
#include <unordered_set>

constexpr float DEGTORAD = 0.0174533f;

class SingleplayerWorld : public World {
public:
	SingleplayerWorld() {

	}
	
	void SetClientSettings(ClientSettings Settings) {
		HorizontalLoadedDistance = Settings.HorizontalRenderDistance;
		VerticalLoadedDistance = Settings.VerticalRenderDistance;

		HorizontalTickingDistance = Settings.HorizontalTickingDistance;
		VerticalLoadedDistance = Settings.VerticalTickingDistance;
	}

	void UpdatePlayer(std::unordered_set<char> KeyInputs, double delta) {
		Entity* player = GetPlayer();
		if (KeyInputs.count('W') || KeyInputs.count('w')) {
			player->Velocity.x += player->Speed * cos(DEGTORAD * player->Rotation.y);
			player->Velocity.z += player->Speed * sin(DEGTORAD * player->Rotation.y);
		}
		if (KeyInputs.count('S') || KeyInputs.count('s')) {
			player->Velocity.x += -player->Speed * cos(DEGTORAD * player->Rotation.y);
			player->Velocity.z += -player->Speed * sin(DEGTORAD * player->Rotation.y);
		}
		if (KeyInputs.count('A') || KeyInputs.count('a')) {
			player->Velocity.x += -player->Speed * cos(DEGTORAD * (player->Rotation.y + 90));
			player->Velocity.z += -player->Speed * sin(DEGTORAD * (player->Rotation.y + 90));
		}
		if (KeyInputs.count('D') || KeyInputs.count('d')) {
			player->Velocity.x += player->Speed * cos(DEGTORAD * (player->Rotation.y + 90));
			player->Velocity.z += player->Speed * sin(DEGTORAD * (player->Rotation.y + 90));
		}
	}


private:
	void MovePlayer() {

	}

	Entity* GetPlayer() {
		return nullptr;
	}
};
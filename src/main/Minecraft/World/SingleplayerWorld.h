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
		if (KeyInputs.count('W') || KeyInputs.count('w')) {
			player->VelocityX += Distance * cos(DEGTORAD * player->RotY);
			player->VelocityZ += Distance * sin(DEGTORAD * player->RotY);
		}
		if (KeyInputs.count('S') || KeyInputs.count('s')) {
			player->VelocityX += -Distance * cos(DEGTORAD * player->RotY);
			player->VelocityZ += -Distance * sin(DEGTORAD * player->RotY);
		}
		if (KeyInputs.count('A') || KeyInputs.count('a')) {
			player->VelocityX += -Distance * cos(DEGTORAD * (player->RotY + 90));
			player->VelocityZ += -Distance * sin(DEGTORAD * (player->RotY + 90));
		}
		if (KeyInputs.count('D') || KeyInputs.count('d')) {
			player->VelocityX += Distance * cos(DEGTORAD * (player->RotY + 90));
			player->VelocityZ += Distance * sin(DEGTORAD * (player->RotY + 90));
		}
	}


private:
	void MovePlayer() {

	}
};
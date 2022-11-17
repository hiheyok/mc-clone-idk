#pragma once

#include "World.h"
#include <string>
#include "../Client/Settings/ClientSettings.h"
class SingleplayerWorld : public World {
public:
	SingleplayerWorld(std::string PlayerName) {

	}
	
	void CopyClientSettings(ClientSettings Settings) {
		HorizontalLoadedDistance = Settings.HorizontalRenderDistance;
		VerticalLoadedDistance = Settings.VerticalRenderDistance;

		HorizontalTickingDistance = Settings.HorizontalTickingDistance;
		VerticalLoadedDistance = Settings.VerticalTickingDistance;
	}
};
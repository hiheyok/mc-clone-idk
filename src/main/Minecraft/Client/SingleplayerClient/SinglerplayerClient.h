#pragma once
#include "../../World/SingleplayerWorld.h"
class SingleplayerClient {
public:
	void JoinWorld(SingleplayerWorld* CurrentWorld_) {
		CurrentWorld = CurrentWorld_;
	}

	void SetWorldSettings(ClientSettings Settings) {
		CurrentWorld->SetClientSettings(Settings);
	}

	void GetPlayerPosition() {
		CurrentWorld->
	}
private:
	SingleplayerWorld* CurrentWorld;
	std::string Username;
};
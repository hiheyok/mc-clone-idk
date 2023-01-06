#pragma once
#include <vector>
#include "../../RenderEngine/Window.h"
#include "ClientStats.h"
#include "../../RenderEngine/GUI/GUI.h"
#include "../World/World.h"
#include <unordered_set>

typedef unsigned int CLIENT_EVENTS;


class Client : Window, ClientStats {
public:

	void Initialize();
	void Render();
	void Update();
	void Stop();
	
private:
	
	void UpdateKeyboardInputs();
	void ClientLoop();
	//Stats
	
	std::vector<int> ClientEvents;
	std::unordered_set<char> KeysPressed;
	IGUI GUI;
};
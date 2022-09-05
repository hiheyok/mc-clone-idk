#pragma once

#include <vector>

#include "../../RenderEngine/Window.h"
#include "../../RenderEngine/GUI/GUI.h"
#include "../World/World.h"
#include <unordered_map>
class Client : Window {
public:

	void Initialize();
	void Render();
	void Update();
	void Stop();
	
private:
	
	void UpdateKeyboardInputs();
	void ClientLoop();
	//Stats
	double frametime = 0;
	int FPS = 0;
	int FPS_LOW = 0;
	int FPS_HIGH = 0;
	std::vector<int> ClientEvents;
	std::unordered_map<char, bool> KeysPressed;

	bool FirstPersonMode = false;

	ClientWorld cworld;
	World TestWorld;
	IGUI GUI;
};
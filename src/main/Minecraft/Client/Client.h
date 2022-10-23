#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
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
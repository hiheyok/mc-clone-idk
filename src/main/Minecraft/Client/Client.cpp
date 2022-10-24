#include "Client.h"
#include "../World/World.h"

#include<iostream>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#define EVENT_NULL 0x0000
#define EVENT_START_INTEGRATED_SERVER 0x0001 //start single player world
#define EVENT_CHANGE_SETTINGS 0x0002
#define DEBUG_NULL 0x0003
#define DEBUG_COLLUSION 0x0004
#define EVENT_DEBUG_GEN_RAND_CHUNK 0x0005

void Client::Initialize() {
	Start();
	
	GUI.init(getWindow(), &ClientEvents);
    GUI.update();

    
    TestWorld.Initialize();
    TestWorld.StartGenThreads(32);

    cworld.Start(getWindow());
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    cworld.AddMeshWorker();
    
    DisableCursor();

    TestWorld.JoinWorld("hiheyok", &cworld);

	ClientLoop();

    
}

void Client::ClientLoop() {
    auto time0 = std::chrono::high_resolution_clock::now();
    long long int FrameCount = 0;
	while (!WindowCloseCheck()) {
        FrameCount++;
        auto time1 = std::chrono::high_resolution_clock::now();

        if ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0 > .15) {
            FPS = (int)((double)FrameCount / ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0));
            FPS_LOW = FPS;
            FPS_HIGH = FPS;
            time0 = std::chrono::high_resolution_clock::now();
            FrameCount = 0;
        }
        
      //  getLogger()->LogDebug("Client","Size of Chunk: " + std::to_string(sizeof(Chunk)));

        
        Update();
        Render();
        Refresh();
       
        frametime = (std::chrono::high_resolution_clock::now() - time1).count() / 1000000000.0;

        if (frametime > 0.5) {
            getLogger()->LogWarn("Client","High Frame Time: " + std::to_string(frametime));
        }

        if (FPS_LOW > (1 / frametime))
            FPS_LOW = (int)(1 / frametime);
        if (FPS_HIGH < (1 / frametime))
            FPS_HIGH = (int(1 / frametime));

        if (TestForKeyInputs(GLFW_KEY_ESCAPE)) {
            _CrtDumpMemoryLeaks();
            return;
        }
        
	}
    
}

void Client::Update() {
    cursormovementx = 0;
    cursormovementy = 0;

   // UpdateWindowName("1.1.0A (DEV) | " + std::to_string(1.0 / frametime));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PollInputs();
    UpdateKeyboardInputs();

    GUI.deleteFromRenderQueue("FPS");
    GUI.addGUIresizedTexturewFont("FPS", std::to_string((int)FPS) + " FPS", vec2(0.6f, 1.f), vec2(-1.2, 0.8), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("FPS");
    GUI.deleteFromRenderQueue("FPS_LOW");
    GUI.addGUIresizedTexturewFont("FPS_LOW", std::to_string((int)FPS_LOW) + " FPS_LOW", vec2(0.6f, 1.f), vec2(-1.2, 0.6), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("FPS_LOW");
    GUI.deleteFromRenderQueue("FPS_HIGH");
    GUI.addGUIresizedTexturewFont("FPS_HIGH", std::to_string((int)FPS_HIGH) + " FPS_HIGH", vec2(0.6f, 1.f), vec2(-1.2, 0.4), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("FPS_HIGH");
    GUI.deleteFromRenderQueue("POS");
    GUI.addGUIresizedTexturewFont("POS", "Pos " + std::to_string(cworld.GetPlayerPos().x) + " " + std::to_string(cworld.GetPlayerPos().y) + " " + std::to_string(cworld.GetPlayerPos().z), vec2(0.8f, 0.6f), vec2(-1.1, 0.2), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("POS");
    GUI.deleteFromRenderQueue("SPEED");
    GUI.addGUIresizedTexturewFont("SPEED", "SPEED " + std::to_string(cworld.GetPlayerSpeed().x) + " " + std::to_string(cworld.GetPlayerSpeed().y) + " " + std::to_string(cworld.GetPlayerSpeed().z), vec2(0.8f, 0.6f), vec2(-1.1, 0.0), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("SPEED");

    GUI.prepareRenderer();
    
    cworld.UpdatePlayer(frametime, KeysPressed, glm::vec2(cursormovementx, cursormovementy));
    cworld.PrepareRenderer();
}

void Client::Render() {
    cworld.Render();
    GUI.Render();
}

void Client::Stop() {

}

void Client::UpdateKeyboardInputs() {
    int SHIFT = false;

    KeysPressed.clear();

    if (TestForKeyInputs(GLFW_KEY_RIGHT_SHIFT) || TestForKeyInputs(GLFW_KEY_LEFT_SHIFT))
        SHIFT = true;

    if (TestForKeyInputs(GLFW_KEY_Q)) {
        if (SHIFT) {
            KeysPressed['q'] = true;
        }
        else {
            KeysPressed['Q'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_W)) {
        if (SHIFT) {
            KeysPressed['w'] = true;
        }
        else {
            KeysPressed['W'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_E)) {
        if (SHIFT) {
            KeysPressed['e'] = true;
        }
        else {
            KeysPressed['E'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_R)) {
        if (SHIFT) {
            KeysPressed['r'] = true;
        }
        else {
            KeysPressed['R'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_T)) {
        if (SHIFT) {
            KeysPressed['t'] = true;
        }
        else {
            KeysPressed['T'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_Y)) {
        if (SHIFT) {
            KeysPressed['y'] = true;
        }
        else {
            KeysPressed['Y'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_U)) {
        if (SHIFT) {
            KeysPressed['u'] = true;
        }
        else {
            KeysPressed['U'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_I)) {
        if (SHIFT) {
            KeysPressed['i'] = true;
        }
        else {
            KeysPressed['I'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_O)) {
        if (SHIFT) {
            KeysPressed['o'] = true;
        }
        else {
            KeysPressed['O'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_P)) {
        if (SHIFT) {
            KeysPressed['p'] = true;
        }
        else {
            KeysPressed['P'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_A)) {
        if (SHIFT) {
            KeysPressed['a'] = true;
        }
        else {
            KeysPressed['A'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_S)) {
        if (SHIFT) {
            KeysPressed['s'] = true;
        }
        else {
            KeysPressed['S'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_D)) {
        if (SHIFT) {
            KeysPressed['d'] = true;
        }
        else {
            KeysPressed['D'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_F)) {
        if (SHIFT) {
            KeysPressed['f'] = true;
        }
        else {
            KeysPressed['F'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_G)) {
        if (SHIFT) {
            KeysPressed['g'] = true;
        }
        else {
            KeysPressed['G'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_H)) {
        if (SHIFT) {
            KeysPressed['h'] = true;
        }
        else {
            KeysPressed['H'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_J)) {
        if (SHIFT) {
            KeysPressed['j'] = true;
        }
        else {
            KeysPressed['J'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_K)) {
        if (SHIFT) {
            KeysPressed['k'] = true;
        }
        else {
            KeysPressed['K'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_L)) {
        if (SHIFT) {
            KeysPressed['l'] = true;
        }
        else {
            KeysPressed['L'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_Z)) {
        if (SHIFT) {
            KeysPressed['z'] = true;
        }
        else {
            KeysPressed['Z'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_X)) {
        if (SHIFT) {
            KeysPressed['x'] = true;
        }
        else {
            KeysPressed['X'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_C)) {
        if (SHIFT) {
            KeysPressed['c'] = true;
        }
        else {
            KeysPressed['C'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_V)) {
        if (SHIFT) {
            KeysPressed['v'] = true;
        }
        else {
            KeysPressed['V'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_B)) {
        if (SHIFT) {
            KeysPressed['b'] = true;
        }
        else {
            KeysPressed['B'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_N)) {
        if (SHIFT) {
            KeysPressed['n'] = true;
        }
        else {
            KeysPressed['N'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_M)) {
        if (SHIFT) {
            KeysPressed['m'] = true;
        }
        else {
            KeysPressed['M'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_SPACE)) {
        KeysPressed[' '] = true;
    }
    if (SHIFT) {
        KeysPressed[0] = true;
    }
}
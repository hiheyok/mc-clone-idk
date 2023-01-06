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
            KeysPressed.insert('q');
        }
        else {
            KeysPressed.insert('Q');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_W)) {
        if (SHIFT) {
            KeysPressed.insert('w');
        }
        else {
            KeysPressed.insert('W');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_E)) {
        if (SHIFT) {
            KeysPressed.insert('e');
        }
        else {
            KeysPressed.insert('E');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_R)) {
        if (SHIFT) {
            KeysPressed.insert('r');
        }
        else {
            KeysPressed.insert('R');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_T)) {
        if (SHIFT) {
            KeysPressed.insert('t');
        }
        else {
            KeysPressed.insert('T');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_Y)) {
        if (SHIFT) {
            KeysPressed.insert('y');
        }
        else {
            KeysPressed.insert('Y');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_U)) {
        if (SHIFT) {
            KeysPressed.insert('u');
        }
        else {
            KeysPressed.insert('U');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_I)) {
        if (SHIFT) {
            KeysPressed.insert('i');
        }
        else {
            KeysPressed.insert('I');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_O)) {
        if (SHIFT) {
            KeysPressed.insert('o');
        }
        else {
            KeysPressed.insert('O');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_P)) {
        if (SHIFT) {
            KeysPressed.insert('p');
        }
        else {
            KeysPressed.insert('P');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_A)) {
        if (SHIFT) {
            KeysPressed.insert('a');
        }
        else {
            KeysPressed.insert('A');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_S)) {
        if (SHIFT) {
            KeysPressed.insert('s');
        }
        else {
            KeysPressed.insert('S');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_D)) {
        if (SHIFT) {
            KeysPressed.insert('d');
        }
        else {
            KeysPressed.insert('D');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_F)) {
        if (SHIFT) {
            KeysPressed.insert('f');
        }
        else {
            KeysPressed.insert('F');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_G)) {
        if (SHIFT) {
            KeysPressed.insert('g');
        }
        else {
            KeysPressed.insert('G');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_H)) {
        if (SHIFT) {
            KeysPressed.insert('h');
        }
        else {
            KeysPressed.insert('H');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_J)) {
        if (SHIFT) {
            KeysPressed.insert('j');
        }
        else {
            KeysPressed.insert('J');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_K)) {
        if (SHIFT) {
            KeysPressed.insert('k');
        }
        else {
            KeysPressed.insert('K');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_L)) {
        if (SHIFT) {
            KeysPressed.insert('l');
        }
        else {
            KeysPressed.insert('L');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_Z)) {
        if (SHIFT) {
            KeysPressed.insert('z');
        }
        else {
            KeysPressed.insert('Z');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_X)) {
        if (SHIFT) {
            KeysPressed.insert('x');
        }
        else {
            KeysPressed.insert('X');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_C)) {
        if (SHIFT) {
            KeysPressed.insert('c');
        }
        else {
            KeysPressed.insert('C');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_V)) {
        if (SHIFT) {
            KeysPressed.insert('v');
        }
        else {
            KeysPressed.insert('V');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_B)) {
        if (SHIFT) {
            KeysPressed.insert('b');
        }
        else {
            KeysPressed.insert('B');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_N)) {
        if (SHIFT) {
            KeysPressed.insert('n');
        }
        else {
            KeysPressed.insert('N');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_M)) {
        if (SHIFT) {
            KeysPressed.insert('m');
        }
        else {
            KeysPressed.insert('M');
        }
    }
    if (TestForKeyInputs(GLFW_KEY_SPACE)) {
        KeysPressed.insert(' ');
    }
    if (SHIFT) {

        KeysPressed.insert(0);
    }
}
#include "Client.h"
#include "../World/World.h"


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
    TestWorld.StartGenThreads(4);

    cworld.Start(getWindow());

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

        PollInputs();
        Update();
        Render();
		Refresh();
       
        frametime = (std::chrono::high_resolution_clock::now() - time1).count() / 1000000000.0;

        if (FPS_LOW > (1 / frametime))
            FPS_LOW = (int)(1 / frametime);
        if (FPS_HIGH < (1 / frametime));
            FPS_HIGH = (int(1 / frametime));
        
	}
}

void Client::Update() {
    GUI.deleteFromRenderQueue("FPS");
    GUI.addGUIresizedTexturewFont("FPS", std::to_string((int)FPS) + " FPS", vec2(0.6f, 1.f), vec2(-1.2, 0.8), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("FPS");
    GUI.deleteFromRenderQueue("FPS_LOW");
    GUI.addGUIresizedTexturewFont("FPS_LOW", std::to_string((int)FPS_LOW) + " FPS_LOW", vec2(0.6f, 1.f), vec2(-1.2, 0.6), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("FPS_LOW");
    GUI.deleteFromRenderQueue("FPS_HIGH");
    GUI.addGUIresizedTexturewFont("FPS_HIGH", std::to_string((int)FPS_HIGH) + " FPS_HIGH", vec2(0.6f, 1.f), vec2(-1.2, 0.4), vec3(255, 0, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 4);
    GUI.addToRenderQueue("FPS_HIGH");

    GUI.prepareRenderer();

    UpdateKeyboardInputs();
    cworld.UpdatePlayer(frametime, KeysPressed);
    cworld.PrepareRenderer();
}

void Client::Render() {
    cworld.Render();
    GUI.Render();
}

void Client::Stop() {

}

void Client::UpdateKeyboardInputs() {
    int R_SHIFT = false;

    KeysPressed.clear();

    if (TestForKeyInputs(GLFW_KEY_RIGHT_SHIFT))
        R_SHIFT = true;

    if (TestForKeyInputs(GLFW_KEY_Q)) {
        if (R_SHIFT) {
            KeysPressed['q'] = true;
        }
        else {
            KeysPressed['Q'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_W)) {
        if (R_SHIFT) {
            KeysPressed['w'] = true;
        }
        else {
            KeysPressed['W'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_E)) {
        if (R_SHIFT) {
            KeysPressed['e'] = true;
        }
        else {
            KeysPressed['E'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_R)) {
        if (R_SHIFT) {
            KeysPressed['r'] = true;
        }
        else {
            KeysPressed['R'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_T)) {
        if (R_SHIFT) {
            KeysPressed['t'] = true;
        }
        else {
            KeysPressed['T'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_Y)) {
        if (R_SHIFT) {
            KeysPressed['y'] = true;
        }
        else {
            KeysPressed['Y'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_U)) {
        if (R_SHIFT) {
            KeysPressed['u'] = true;
        }
        else {
            KeysPressed['U'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_I)) {
        if (R_SHIFT) {
            KeysPressed['i'] = true;
        }
        else {
            KeysPressed['I'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_O)) {
        if (R_SHIFT) {
            KeysPressed['o'] = true;
        }
        else {
            KeysPressed['O'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_P)) {
        if (R_SHIFT) {
            KeysPressed['p'] = true;
        }
        else {
            KeysPressed['P'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_A)) {
        if (R_SHIFT) {
            KeysPressed['a'] = true;
        }
        else {
            KeysPressed['A'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_S)) {
        if (R_SHIFT) {
            KeysPressed['s'] = true;
        }
        else {
            KeysPressed['S'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_D)) {
        if (R_SHIFT) {
            KeysPressed['d'] = true;
        }
        else {
            KeysPressed['D'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_F)) {
        if (R_SHIFT) {
            KeysPressed['f'] = true;
        }
        else {
            KeysPressed['F'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_G)) {
        if (R_SHIFT) {
            KeysPressed['g'] = true;
        }
        else {
            KeysPressed['G'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_H)) {
        if (R_SHIFT) {
            KeysPressed['h'] = true;
        }
        else {
            KeysPressed['H'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_J)) {
        if (R_SHIFT) {
            KeysPressed['j'] = true;
        }
        else {
            KeysPressed['J'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_K)) {
        if (R_SHIFT) {
            KeysPressed['k'] = true;
        }
        else {
            KeysPressed['K'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_L)) {
        if (R_SHIFT) {
            KeysPressed['l'] = true;
        }
        else {
            KeysPressed['L'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_Z)) {
        if (R_SHIFT) {
            KeysPressed['z'] = true;
        }
        else {
            KeysPressed['Z'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_X)) {
        if (R_SHIFT) {
            KeysPressed['x'] = true;
        }
        else {
            KeysPressed['X'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_C)) {
        if (R_SHIFT) {
            KeysPressed['c'] = true;
        }
        else {
            KeysPressed['C'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_V)) {
        if (R_SHIFT) {
            KeysPressed['v'] = true;
        }
        else {
            KeysPressed['V'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_B)) {
        if (R_SHIFT) {
            KeysPressed['b'] = true;
        }
        else {
            KeysPressed['B'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_N)) {
        if (R_SHIFT) {
            KeysPressed['n'] = true;
        }
        else {
            KeysPressed['N'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_M)) {
        if (R_SHIFT) {
            KeysPressed['m'] = true;
        }
        else {
            KeysPressed['M'] = true;
        }
    }
    if (TestForKeyInputs(GLFW_KEY_SPACE)) {
        KeysPressed[' '] = true;
    }
}
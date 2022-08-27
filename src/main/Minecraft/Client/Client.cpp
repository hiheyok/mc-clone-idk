#include "Client.h"

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

    mineplusplus.Init();

	ClientLoop();
}

void Client::ClientLoop() {
    auto time0 = std::chrono::high_resolution_clock::now();
    long long int FrameCount = 0;
	while (!WindowCloseCheck()) {
        FrameCount++;
        auto time1 = std::chrono::high_resolution_clock::now();

        if ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0 > .15) {
            FPS = (double)FrameCount / ((std::chrono::high_resolution_clock::now() - time0).count() / 1000000000.0);
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
       
        double frametime = (std::chrono::high_resolution_clock::now() - time1).count() / 1000000000.0;

        if (FPS_LOW > (1 / frametime))
            FPS_LOW = (1 / frametime);
        if (FPS_HIGH < (1 / frametime))
            FPS_HIGH = (1 / frametime);
        
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

    int R_SHIFT = false;

    if (TestForKeyInputs(GLFW_KEY_RIGHT_SHIFT))
        R_SHIFT = true;
}

void Client::Render() {
    GUI.Render();
}

void Client::Stop() {

}
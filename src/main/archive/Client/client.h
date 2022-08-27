#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <thread>
#include "renderer/camera/camera.h"
#include "../utils/functions/logging/log.h"
#include "../Server/server.h"
#include "clientworld/cworld.h"
#include "renderer/gui/IGUI/IGUI.h"
#include "renderer/ChunkRenderer.h"

#include "EventContainer.h"

class Client {
public:

	void start();

	void initialize(); 
	void stop() {
		logs->stopSignal = true;
	}

	void mainGameLoop();
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void resize_window(GLFWwindow* window, int x, int y);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void updateTitleStats();
	void EventCall();
	void keycallback();
	void Reload();
	void RenderWorld();
	void getStats();

	std::vector<int> CLIENT_EVENTS;

	void render();
	void IO();
	GLFWwindow* window = nullptr;

private:

	ClientWorld cworld;

	ChunkRenderer WorldRenderer;

	TexturedFrameBuffer worldRender;

	Server* server = nullptr;
	Camera* camera = nullptr;
	LOG* logs = nullptr;
	IGUI* gui = nullptr;
	//Callbacks & IO

	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	int sizex = 1280;
	int sizey = 720;

	bool firstMouse = true;

	//cam

	double lastX = 1920 / 2.0f;
	double lastY = 1080 / 2.0f;

	double fps = 0;

	double delta = 0;
	double oframe = 0;
	double cframe = 0;

	int maxFPS = 360;

	bool playMode = false;

	// DEBUG OPTIONS

	bool OPENGL_EXTENSIONS_TEST = false;

	std::thread threads[16];

};





#endif
#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "../utils/LogUtils.h"
#include <vector>


class Window {
public:
	void Start();

	int cursorx = 0;
	int cursory = 0;

	void Refresh();
	void PollInputs();
	GLFWwindow* getWindow();

protected:
	bool WindowCloseCheck();
	void DisableCursor();
	void EnableCursor();

	void Typing();

	bool TestForKeyInputs(int Key);

private:
	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	void resize_window(int x, int y);
	void mouse_callback(double xpos, double ypos);
	int sizex = 1280;
	int sizey = 720;

	LogUtils* Logger;
	GLFWwindow* window;
	bool init = false;
};
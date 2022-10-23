#pragma once
#ifndef IGUI_H
#define IGUI_H
#define _CRTDBG_MAP_ALLOC
#include <unordered_map>
#include "../OpenGL/Shader/Shader.h"
#include "../OpenGL/Texture/Texture.h"
#include "../OpenGL/Framebuffer/Framebuffer.h"
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <chrono>
#include <vector>

using namespace glm;


struct GUI_Data {
	std::string name;
	vec2 p0, p1, p2, p3 = vec2(0.0f, 0.0f);
	vec2 t0, t1, t2, t3 = vec2(0.0f, 0.0f);
	vec3 color;
	bool clickable = false;
	bool isSlider = false;
	std::string text;
	int Slider_MAX = 0;
	int Slider_MIN = 0;
	int Slider_CVAL = 0;
	std::vector<std::string> guiDel;
	std::vector<std::string> guiAdd;
	bool render = false;
	int gevent = NULL;
	unsigned int renderstep;
};

struct LETTER {
	vec2 p0 = vec2(0.0f, 0.0f), p1 = vec2(0.0f, 0.0f), p2 = vec2(0.0f, 0.0f), p3 = vec2(0.0f,0.0f);
	vec2 t0 = vec2(0.0f, 0.0f), t1 = vec2(0.0f, 0.0f), t2 = vec2(0.0f, 0.0f), t3 = vec2(0.0f, 0.0f);
};

struct string_rendering_data {
	std::vector<LETTER> string;
	float letterSize = 0.f;
	vec3 color = vec3(0.f,0.f,0.f);
	bool render = false;
};


class IGUI {
public:


	void init(GLFWwindow* window_, std::vector<int>* CE);

	void addGUI(std::string name, vec2 size, vec2 pos, vec3 color, vec2 tpos0, vec2 tpos1, bool clickable, int step);
	void addGUIresizedTexture(std::string name, vec2 size, vec2 pos, vec3 color, vec2 tpos0, vec2 tpos1, vec2 textureSize, bool clickable, int step);
	void addGUIresizedTexturewFont(std::string name, std::string str, vec2 size, vec2 pos, vec3 color, vec2 tpos0, vec2 tpos1, vec2 textureSize, bool clickable, int step);

	void addToRenderQueue(std::string name);
	void deleteFromRenderQueue(std::string name);

	void addToRenderQueue(std::vector<std::string> name);
	void deleteFromRenderQueue(std::vector<std::string> name);

	void Render();

	void update();

	void changeGUIcall(std::string name, std::vector<std::string> add, std::vector<std::string> del);
	void changeGUIEvent(std::string name, int EVENT);

	void configTexture(int renderStep, std::string tdir, int index);
	void configTexture(int renderStep, TexturedFrameBuffer fbo, int index);

	glm::vec4 getCharTextCoords(const char letter);

	glm::vec4 getPixelCoords(glm::vec2 TextureSize, glm::vec2 lu, glm::vec2 rd);
	glm::vec4 getPixelCoordsOffset(glm::vec2 TextureSize, glm::vec2 lu, glm::vec2 offset);

	GUI_Data* getGUI(std::string name);

	Shader guiShader[16]{};

	glm::vec2 MouseLeftButtonPressPos;
	glm::vec2 MouseRightButtonPressPos;
	glm::vec2 MousePos;

	std::vector<int>* CLIENT_EVENTS;
	GLFWwindow* window;

	bool clicked;

	int debug0, debug1, debug2, debug3;
	
	int RenderingStepsCount = 1;
	void prepareRenderer();
private:

	std::chrono::steady_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	void clickDetection();

	

	void tick();

	Texture texture[16]{};
	Texture font;

	unsigned int VBO[16]{}, VAO[16]{}, EBO[16]{}, size[16]{};

	unsigned int fontVBO, fontVAO, fontEBO;

	size_t fontsize;

	std::unordered_map<std::string, GUI_Data> data;
	std::unordered_map<std::string, string_rendering_data> gui_string;
	std::vector<std::string> tickable;



};

#endif
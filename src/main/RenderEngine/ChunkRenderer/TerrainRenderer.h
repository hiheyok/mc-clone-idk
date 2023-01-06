#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh/Mesher.h"

#include "../Camera/camera.h"
#include "../frustum/frustum.h"
#include "../OpenGL/shader/shader.h"
#include "../OpenGL/Texture/texture.h"
#include "../../Utils/MathHelper.h"
#include "../../Utils/MutithreadedData.h"
#include "../../Minecraft/Core/Registry.h"
#include "../OpenGL/Buffers/Buffer.h"
#include <unordered_map>
#include <deque>
#include <queue>



class TerrainRenderer {
public:
	TerrainRenderer() {

	}

	void Initialize(GLFWwindow* window_) {
		window = window_;
		SetupShaders();
	}

private:
	void SetupShaders() {
		SolidShader->init("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
	}

	GLFWwindow* window = nullptr;
	
	Shader* SolidShader;
};


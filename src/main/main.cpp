
#include <thread>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
//#include "RenderEngine\OpenGL\Texture\stb_image.h"
#include <iostream>
#include "RenderEngine/Window.h"
#include "Utils/LogUtils.h"
#include "Minecraft/Core.h"
#include "Minecraft/Client/Client.h"

int main(void) {

	LogUtils* Logger = getLogger();

	Logger->Start();

	int test = 0;

	Client GameClient;
	GameClient.Initialize();
	
	return 0x01;
//	std::this_thread::sleep_for(std::chrono::hours(9999999));
}
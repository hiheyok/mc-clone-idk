#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "../World/Level/Block/Block.h"
#include "../World/Level/Dimension/Dimension.h"
#include "../World/Entity/Entity.h"
#include "../../RenderEngine/OpenGL/Texture/Texture.h"
#include "../../Utils/LogUtils.h"
#include <unordered_map>
#include <string>


typedef unsigned int ID;

//container for all of the content in the game
template <class T>
class Registry {

public:
	std::string NAMESPACE = "minecpp";
	std::string NAMESPACE_SEPARATER = ":";
	ID Register(T obj, std::string name) {
		Container[nextID] = T;
		NameContainer[nextID] = NAMESPACE + NAMESPACE_SEPARATER + name;
		nextID++;
		return nextID - 1;
	}

	ID RegisterNULL(std::string name) {
		Container[nextID];
		NameContainer[nextID] = NAMESPACE + NAMESPACE_SEPARATER + name;
		nextID++;
		return nextID;
	}

	T& operator[](ID id) {
		return Container[id];
	}

	T& getObject(ID id) {
		return Container[id];
	}

	std::string& getName(ID id) {
		return NameContainer[id];
	}
	std::unordered_map<ID, T> Container;
private:

	
	std::unordered_map < ID, std::string > NameContainer;
	
	ID nextID = NULL;
};

Registry<Block*> BlockRegistry;
Registry<std::string> BlockIMGRegistry;
Registry<Texture*> TextureRegistry;
Registry<Dimension*> DimensionRegistry;
Registry<Entity*> EntityRegistry;
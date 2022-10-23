#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <unordered_map>
#include <string>

//container for all of the content in the game

class Registry { 
	
public:

	void CreateRegistry();
	void GetID();
	void Add(long long RegID, std::string info);

	std::unordered_map<unsigned long, std::string> EntityID;

private:

	std::unordered_map<unsigned long, std::unordered_map<long long, long long>> Container;
	std::unordered_map<unsigned long, std::string> StringMap;
	
	long long int ObjCount = 0;
};
#pragma once
#include <string>
#include "BlockBehavior.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

typedef unsigned short int BlockID;

class BlockStatus : public BlockBehavior {
public:
	void SetID(BlockID ID_) {
		ID = ID_;
	}
private:
	BlockID ID = NULL;

};
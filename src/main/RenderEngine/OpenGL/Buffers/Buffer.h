#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#define _CRTDBG_MAP_ALLOC
#include <vector>

class Buffer {
public:
	Buffer() {
		glGenBuffers(1, &BufferID);
	}

	unsigned int GetID();

	void InsertData(GLenum BufferType, GLsizeiptr Size, const void* Data, GLenum Usage);
	void InsertSubData(GLenum BufferType, GLintptr Offset, GLsizeiptr Size, const void* Data);
private:
	unsigned int BufferID = NULL;
};
#include "Buffer.h"
#define _CRTDBG_MAP_ALLOC
unsigned int Buffer::GetID() {
	return BufferID;
}
void Buffer::InsertData(GLsizeiptr Size, const void* Data, GLenum Usage) {
	glBindBuffer(Type, BufferID);
	glBufferData(Type, Size, Data, Usage);
	glBindBuffer(Type, 0);
}

void Buffer::InsertSubData(GLintptr Offset, GLsizeiptr Size, const void* Data) {
	glBindBuffer(Type, BufferID);
	glBufferSubData(Type,Offset, Size, Data);
	glBindBuffer(Type, 0);
}

void Buffer::ResetBuffer() {
	glDeleteBuffers(1, &BufferID);
	glGenBuffers(1, &BufferID);
}

void Buffer::Bind() {
	glBindBuffer(Type, BufferID);
}

void Buffer::Unbind() {
	glBindBuffer(Type, 0);
}

void Buffer::Delete() {
	glDeleteBuffers(1, &BufferID);
	BufferID = NULL;
	MaxSize = NULL;
	Type = NULL;
}

void Buffer::SetMaxSize(size_t MaxSize_) {
	MaxSize = MaxSize_;
}

void Buffer::SetType(GLenum Type_) {
	Type = Type_;
}

void Buffer::SetUsage(GLenum Usage_) {
	Usage = Usage_;
}

void Buffer::InitializeData() {
	Bind();
	glBufferData(Type, MaxSize, nullptr, Usage);
	Unbind();
}

void VertexArray::Bind() {
	glBindVertexArray(ArrayID);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}

void VertexArray::EnableAttriPTR(GLuint Index, GLint Size, GLenum Type, GLboolean normalized, GLsizei Stride, int SubIndex) {
	glVertexAttribPointer(Index, Size, Type, normalized, sizeof(Type) * Stride, (void*)(SubIndex * sizeof(unsigned int)));
	glEnableVertexAttribArray(Index);
}

void VertexArray::ResetArray() {
	glDeleteVertexArrays(1, &ArrayID);
	glGenVertexArrays(1, &ArrayID);
}
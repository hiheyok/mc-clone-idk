#include "Buffer.h"

unsigned int Buffer::GetID() {
	return BufferID;
}
void Buffer::InsertData(GLenum BufferType, GLsizeiptr Size, const void* Data, GLenum Usage) {
	glBindBuffer(BufferType, BufferID);
	glBufferData(BufferType, Size, Data, Usage);
	glBindBuffer(BufferType, 0);
}

void Buffer::InsertSubData(GLenum BufferType, GLintptr Offset, GLsizeiptr Size, const void* Data) {
	glBindBuffer(BufferType, BufferID);
	glBufferSubData(BufferType,Offset, Size, Data);
	glBindBuffer(BufferType, 0);
}
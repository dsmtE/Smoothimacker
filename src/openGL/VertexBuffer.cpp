#include "VertexBuffer.hpp"

using namespace openGL;

VertexBuffer::VertexBuffer() {
    GLCall(glGenBuffers(1, &_index));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &_index));
}

void VertexBuffer::bind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _index));
}

void VertexBuffer::unbind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::setVertexAttrib(const GLuint &VertexBufferID, const GLuint &size, GLenum &dataType, const GLuint &stride, const GLuint &offset) {
	GLCall(glEnableVertexAttribArray(VertexBufferID));
	GLCall(glVertexAttribPointer(
		VertexBufferID,
		size,
		dataType,
		GL_FALSE,
		stride,  // stride
		(const GLvoid*)(offset)   // pointer
	));
}
void VertexBuffer::setVertexAttribInteger(GLuint &VertexBufferID,const GLuint &size, GLenum &dataType,const GLuint &stride,const GLuint &offset) {
	GLCall(glEnableVertexAttribArray(VertexBufferID));
	GLCall(glVertexAttribIPointer(
		VertexBufferID,
		size,
		dataType,
		stride,  // stride
		(const GLvoid*)(offset)   // pointer
	));
}


void VertexBuffer::setAttribDivisor(const GLuint& index, const GLubyte& divisor) {
	GLCall(glVertexAttribDivisor(index, divisor));
}

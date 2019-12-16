#include "ElementBuffer.hpp"

using namespace openGL;

ElementBuffer::ElementBuffer() {
     GLCall(glGenBuffers(1, &_index));
}

ElementBuffer::~ElementBuffer() {
     GLCall(glDeleteBuffers(1, &_index));
}

void ElementBuffer::bind() {
     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index));
}

void ElementBuffer::unbind() {
     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void ElementBuffer::setData(const std::vector<glm::uvec3>& indices, const GLenum &drawType) {
     bind();
     GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * indices.size(), indices.data(),  drawType));
     unbind();
}
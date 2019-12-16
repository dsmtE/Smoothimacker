#include "VertexArray.hpp"
#include <glad/glad.h>

using namespace openGL;

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &_index));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &_index));
}

void VertexArray::bind() {
    GLCall(glBindVertexArray(_index));
}

void VertexArray::unbind() {
    GLCall(glBindVertexArray(0));
}

#include "Grid.hpp"

#include <iostream>

using namespace world;

Grid::Grid(const unsigned int &size, const glm::vec3 &color) : _size(size), _color(color) {

    _vertices = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(1, 1),
        glm::vec2(0, 1),
        glm::vec2(0, 0) 
    };
	
	_VBO.setData<glm::vec2>(_vertices, GL_STATIC_DRAW);
	_VAO.bind();
	_VBO.bind();
	openGL::VertexBuffer::setVertexAttrib(0, 3, GL_FLOAT, sizeof(glm::vec2), 0);
	_VBO.unbind();
    _VAO.unbind(); 
}

void Grid::draw(const Camera &cam, openGL::Shader &shader) {
    shader.bind();
    // send uniform to shader
    shader.setVec3f("gridColor", _color);
    shader.setMat4("MVPMatrix", cam.PVMatrix());
    shader.setInt("size", _size);
    _VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    _VAO.unbind();
    shader.unBind();
}
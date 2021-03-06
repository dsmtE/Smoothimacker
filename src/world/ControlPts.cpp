#include "ControlPts.hpp"
#include <algorithm> // for std::remove_if

using namespace world;

ControlPts::ControlPts(const glm::vec3 &color) : _color(color) {
    //    v5----- v4
    //   /|      /|
    //  v0------v1|
    //  | |     | |
    //  | |v6---|-|v7
    //  |/      |/
    //  v3------v2

    const glm::vec3 v0 = glm::vec3(-1, 1, 1) * 0.5f;
    const glm::vec3 v1 = glm::vec3(1, 1, 1) * 0.5f;
    const glm::vec3 v2 = glm::vec3(1, -1, 1) * 0.5f;
    const glm::vec3 v3 = glm::vec3(-1, -1, 1) * 0.5f;
    const glm::vec3 v4 = glm::vec3(1, 1, -1) * 0.5f;
    const glm::vec3 v5 = glm::vec3(-1, 1, -1) * 0.5f;
    const glm::vec3 v6 = glm::vec3(-1, -1, -1) * 0.5f;
    const glm::vec3 v7 = glm::vec3(1, -1, -1) * 0.5f;

    _cubeVertices = {
        v0, v1, v2, v3, //front
        v0, v5, v6, v3, // left
        v2, v7, v4, v1, // right
        v4, v5, v6, v7, // back
    };

	_VBO.setData<glm::vec3>(_cubeVertices, GL_STATIC_DRAW);

    _instanceVBO.setData<glm::vec3>(_controlCubesPos, GL_STATIC_DRAW);

    const GLuint vertexAttrib_cubeVertexRelativePos = 0;
    const GLuint vertexAttrib_cubePosition = 1;
	
	_VAO.bind();
	_VBO.bind();
	openGL::VertexBuffer::setVertexAttrib(vertexAttrib_cubeVertexRelativePos, 3, GL_FLOAT, sizeof(glm::vec3), 0);
	_VBO.unbind();
    // bind instance position VBO
     _instanceVBO.bind();
	openGL::VertexBuffer::setVertexAttrib(vertexAttrib_cubePosition, 3, GL_FLOAT, sizeof(glm::vec3), 0);
    _instanceVBO.setAttribDivisor(vertexAttrib_cubePosition, 1); // tell OpenGL this is an instanced vertex attribute.
	_instanceVBO.unbind();
    _VAO.unbind();

    addControlPts(glm::vec3(0.0f, 0.0f, 0.0f));
}   

void ControlPts::draw(const Camera &cam, openGL::Shader &shader) {
    shader.bind();
    // send uniform to shader
    shader.setVec3f("controlPtsColor", _color);
    shader.setMat4("MVPMatrix", cam.PVMatrix());

    _VAO.bind();
    glDrawArraysInstanced(GL_LINE_STRIP, 0, _cubeVertices.size() ,_controlCubesPos.size());
    _VAO.unbind();
    shader.unBind();
}

void ControlPts::addControlPts(const glm::vec3 &p) {
    _controlCubesPos.push_back(p);
    _instanceVBO.setData<glm::vec3>(_controlCubesPos, GL_STATIC_DRAW);
}
bool ControlPts::delControlPts(const glm::vec3 &p) {
    const size_t tempSize = _controlCubesPos.size();
    _controlCubesPos.erase(std::remove_if( _controlCubesPos.begin(), _controlCubesPos.end(),
    [p](const glm::vec3 &pos) { 
        return pos == p; 
    }), _controlCubesPos.end());
    if(tempSize != _controlCubesPos.size()) {
        _instanceVBO.setData<glm::vec3>(_controlCubesPos, GL_STATIC_DRAW);
        return true;
    }
    return false;
    
}

void ControlPts::resetControlPts() {
    _controlCubesPos.clear();
    _instanceVBO.setData<glm::vec3>(_controlCubesPos, GL_STATIC_DRAW);
}

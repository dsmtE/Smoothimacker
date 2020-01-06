#include "PointsLights.hpp"

using namespace world;
#include <algorithm> // for std::remove_if

PointsLights::PointsLights() {

    //    v5----- v4
    //   /|      /|
    //  v0------v1|
    //  | |     | |
    //  | |v6---|-|v7
    //  |/      |/
    //  v3------v2

    glm::vec3 v0 = glm::vec3(-1, 1, 1) * 0.2f;
    glm::vec3 v1 = glm::vec3(1, 1, 1) * 0.2f;
    glm::vec3 v2 = glm::vec3(1, -1, 1) * 0.2f;
    glm::vec3 v3 = glm::vec3(-1, -1, 1) * 0.2f;
    glm::vec3 v4 = glm::vec3(1, 1, -1) * 0.2f;
    glm::vec3 v5 = glm::vec3(-1, 1, -1) * 0.2f;
    glm::vec3 v6 = glm::vec3(-1, -1, -1) * 0.2f;
    glm::vec3 v7 = glm::vec3(1, -1, -1) * 0.2f;

    
    _displayCubeVertices = { 
        v0,v1,v2,v3, //face
        v4,v5,v6,v7, // back
        v5,v0,v3,v6, // left
        v1,v4,v7,v2, // right
        v5,v4,v1,v0, // top
        v3,v2,v7,v6 // bottom
    };

	_displayCubeIndices = {
		 glm::uvec3(0, 1, 2),   glm::uvec3(2, 3, 0),     // front
		 glm::uvec3(4, 5, 6),   glm::uvec3(6, 7, 4),     // right
		 glm::uvec3(8, 9, 10),  glm::uvec3(10,11,8),     // top
		 glm::uvec3(12,13,14),  glm::uvec3(14,15,12),   // left
		 glm::uvec3(16,17,18),  glm::uvec3(18,19,16),   // bottom
		 glm::uvec3(20,21,22),  glm::uvec3(22,23,20)    // back
	};

    _EBO.setData(_displayCubeIndices, GL_STATIC_DRAW);

	_VBO.setData<glm::vec3>(_displayCubeVertices, GL_STATIC_DRAW);

    const GLuint vertexAttrib_Pos = 0;
    const GLuint vertexAttrib_lightPos = 1;
	
	_VAO.bind();
    _EBO.bind();
	_VBO.bind();
	openGL::VertexBuffer::setVertexAttrib(vertexAttrib_Pos, 3, GL_FLOAT, sizeof(glm::vec3), 0);
	_VBO.unbind();

    // bind instance position VBO
    _lightPosVBO.bind();
    openGL::VertexBuffer::setVertexAttrib(vertexAttrib_lightPos, 3, GL_FLOAT, sizeof(PointLight), offsetof(PointLight, pos));
    _lightPosVBO.setAttribDivisor(vertexAttrib_lightPos, 1); // tell OpenGL this is an instanced vertex attribute.
	_lightPosVBO.unbind();
    _VAO.unbind();
}

void PointsLights::draw(const Camera &cam, openGL::Shader &shader) {
    shader.bind();
    shader.setMat4("MVPMatrix", cam.PVMatrix());

    // set struct value using uniform
    for (GLuint i = 0; i < _lights.size(); i++) {
        std::string iStr = std::to_string(i);
        shader.setVec3f("pointsLightsAmbiantColor[" + iStr + "]", _lights[i].ambientColor);
    }

    _VAO.bind();
    glDrawElementsInstanced(GL_TRIANGLES, _displayCubeIndices.size() * 3, GL_UNSIGNED_INT , 0, _lights.size());
    _VAO.unbind();
    shader.unBind();
}

void PointsLights::addPointLight(const glm::vec3 &p, const float &constant, const float &linear, const float &quadratic, const  glm::vec3 &ambientColor, const  glm::vec3 &diffuseColor) {
    _lights.push_back(PointLight{p, constant, linear, quadratic, ambientColor, diffuseColor});
    _lightPosVBO.setData<PointLight>(_lights, GL_STATIC_DRAW);
}

void PointsLights::resetPointLights() {
    _lights.clear();
    _lightPosVBO.setData<PointLight>(_lights, GL_STATIC_DRAW);
}

void PointsLights::delPointLight(const glm::vec3 &p) {
    _lights.erase(std::remove_if( _lights.begin(), _lights.end(),
    [p](const PointLight &pl) { 
        return pl.pos == p; 
    }), _lights.end());
    _lightPosVBO.setData<PointLight>(_lights, GL_STATIC_DRAW);
}
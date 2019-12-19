#include "Cursor.hpp"

#include <iostream>

using namespace world;

Cursor::Cursor(const glm::vec3 &color) : _position(0,0,0), _color(color) {

    //    v5----- v4
    //   /|      /|
    //  v0------v1|
    //  | |     | |
    //  | |v6---|-|v7
    //  |/      |/
    //  v3------v2

    const glm::vec3 v0 = glm::vec3(-1, 1, 1);
    const glm::vec3 v1 = glm::vec3(1, 1, 1);
    const glm::vec3 v2 = glm::vec3(1, -1, 1);
    const glm::vec3 v3 = glm::vec3(-1, -1, 1);
    const glm::vec3 v4 = glm::vec3(1, 1, -1);
    const glm::vec3 v5 = glm::vec3(-1, 1, -1);
    const glm::vec3 v6 = glm::vec3(-1, -1, -1);
    const glm::vec3 v7 = glm::vec3(1, -1, -1);

    _vertices = {
        //front
        v0, v1, v2, v3,
        // left
        v0, v5, v6, v3,
        // right
        v2, v7, v4, v1,
        // back
        v4, v5, v6, v7,
    };
	
	_VBO.setData<CursorVertex>(_vertices, GL_STATIC_DRAW);

    const GLuint vertexAttrib_Pos = 0;
	
	_VAO.bind();  // Bind the Vertex Array Object first, then bind VBO and attribute pointer(s).

	_VBO.bind();
	openGL::VertexBuffer::setVertexAttrib(vertexAttrib_Pos, 3, GL_FLOAT, sizeof(CursorVertex), offsetof(CursorVertex, position));
	_VBO.unbind();

    _VAO.unbind(); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void Cursor::draw(const world::Camera &cam, const int& screenWidth, const int& screenHeigh, openGL::Shader &shader) {
    shader.bind();
    // send uniform to shader
    shader.setVec3f("cursorColor", _color);
    shader.setMat4("MVPMatrix", cam.PVMatrix(screenWidth, screenHeigh) * modelMatrix());

    _VAO.bind();
    glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());
    _VAO.unbind();

    shader.unBind();
}
      
glm::mat4 Cursor::modelMatrix() const {
    glm::mat4 m(1.0f);
    m = glm::translate(m, glm::vec3(_position));
    return m;
}

void Cursor::handleEvent(SDL_Event sdlEvent) {
    if (sdlEvent.type == SDL_KEYDOWN) {
        if (sdlEvent.key.keysym.sym == SDLK_RIGHT) {
            _position.x += 1;
        }
        if (sdlEvent.key.keysym.sym == SDLK_LEFT) {
            _position.x -= 1;
        }
        if (sdlEvent.key.keysym.sym == SDLK_UP) {
            _position.y += 1;
        }
        if (sdlEvent.key.keysym.sym == SDLK_DOWN) {
            _position.y -= 1;
        }
        if (sdlEvent.key.keysym.sym == SDLK_KP_PLUS) {
            _position.z += 1;
        }
        if (sdlEvent.key.keysym.sym == SDLK_KP_MINUS) {
            _position.z -= 1;
        }
    }
}

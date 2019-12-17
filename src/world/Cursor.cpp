#include "Cursor.hpp"

#include <iostream>

using namespace world;

Cursor::Cursor() : _position(0,0,0) {

    calcVertices(); // construct _vertices & normal
	
	_VBO.setData<CursorVertex>(_vertices, GL_STATIC_DRAW);

    const GLuint vertexAttrib_Pos = 0;
    const GLuint vertexAttrib_Color = 1;
	
	_VAO.bind();  // Bind the Vertex Array Object first, then bind VBO and attribute pointer(s).

	_VBO.bind();
	openGL::VertexBuffer::setVertexAttrib(vertexAttrib_Pos, 3, GL_FLOAT, sizeof(CursorVertex), offsetof(CursorVertex, position));
    openGL::VertexBuffer::setVertexAttrib(vertexAttrib_Color, 3, GL_FLOAT, sizeof(CursorVertex), offsetof(CursorVertex, color));
	_VBO.unbind();

    _VAO.unbind(); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void Cursor::draw(const openGL::Shader &shader) {
    shader.bind(); // bind shader
    _VAO.bind();
    glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());
    _VAO.unbind();
}

//    v5----- v4
//   /|      /|
//  v0------v1|
//  | |     | |
//  | |v6---|-|v7
//  |/      |/
//  v3------v2
//
//        (Y)
//         ^
//         |
//         .---->(X)
//        /
//       v
//     (Z)
//     
const glm::vec3 v0 = glm::vec3(-1, 1, 1);
const glm::vec3 v1 = glm::vec3(1, 1, 1);
const glm::vec3 v2 = glm::vec3(1, -1, 1);
const glm::vec3 v3 = glm::vec3(-1, -1, 1);
const glm::vec3 v4 = glm::vec3(1, 1, -1);
const glm::vec3 v5 = glm::vec3(-1, 1, -1);
const glm::vec3 v6 = glm::vec3(-1, -1, -1);
const glm::vec3 v7 = glm::vec3(1, -1, -1);

const glm::vec3 colorVec = glm::vec3(0, 0, 1);
      
void Cursor::calcVertices() {

    
    _vertices = {
        //front
        {v0, colorVec},
        {v1, colorVec},
        {v2, colorVec},
        {v3, colorVec},

        // left
        {v0, colorVec},
        {v5, colorVec},
        {v6, colorVec},
        {v3, colorVec},

        // right
        {v2, colorVec},
        {v7, colorVec},
        {v4, colorVec},
        {v1, colorVec},

        // back
        {v4, colorVec},
        {v5, colorVec},
        {v6, colorVec},
        {v7, colorVec},

    };

}

glm::mat4 Cursor::getModelMatrix() const {
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

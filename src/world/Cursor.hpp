#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>

#include "../openGL/glException.hpp"

#include "../openGL/ElementBuffer.hpp"
#include "../openGL/VertexBuffer.hpp"
#include "../openGL/VertexArray.hpp"

#include "../openGL/Shader.hpp"
#include "../world/Camera.hpp"

namespace world {

    struct CursorVertex {
        glm::vec3 position;

        CursorVertex() = default;
        CursorVertex(glm::vec3 const &p) : position(p) {}
    };

    class Cursor {

    private:

        std::vector<CursorVertex> _vertices;
        glm::ivec3 _position;
        glm::vec3 _color;

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO;

    public:

        Cursor(const glm::vec3 &color = glm::vec3(0, 0, 1));
        ~Cursor() = default;
        
        void draw(const world::Camera &cam, const int& screenWidth, const int& screenHeigh, openGL::Shader &shader);

        glm::mat4 modelMatrix() const;

        void handleEvent(SDL_Event sdlEvent);

        inline glm::ivec3* getPointerPos() { return &_position; };
        inline void setPos(const glm::ivec3 p) { _position = p; };
    };
}
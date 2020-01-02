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

    class Grid {

    private:

        std::vector<glm::vec2> _vertices;
        glm::vec3 _color;
        unsigned int _size; 

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO;


    public:

        Grid(const unsigned int &size, const glm::vec3 &color = glm::vec3(0.3f));
        ~Grid() = default;
        void draw(const Camera &cam, openGL::Shader &shader);
        void handleEvent(SDL_Event sdlEvent);
    };
}
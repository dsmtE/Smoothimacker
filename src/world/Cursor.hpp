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

namespace world {

    struct CursorVertex {
        glm::vec3 position;
        glm::vec3 color;

        CursorVertex() = default;
        CursorVertex(glm::vec3 const &p, glm::vec3 const &c)
            : position(p), color(c){
        }
    };

    class Cursor {

    private:

        std::vector<CursorVertex> _vertices;
        glm::ivec3 _position;

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO;

        ///
        ///\brief 
        ///
        void calcVertices();

    public:

        Cursor();
        ~Cursor() = default;
        

        void draw(const openGL::Shader& shader);

        glm::mat4 getModelMatrix() const;

        void handleEvent(SDL_Event sdlEvent);

        inline glm::ivec3 getPos() const { return _position; };
    };
}
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
    ///\brief Cursor class
    class Cursor {

    private:

        std::vector<glm::vec3> _vertices;
        glm::ivec3 _position;
        glm::vec3 _color;

        glm::ivec3 _minLimit, _maxLimit;

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO;

        Camera* _camReferenceForMove;

    public:

        Cursor(const unsigned int &minLimit, const unsigned int &maxLimit, const glm::vec3 &color = glm::vec3(1, 0, 0));
        ~Cursor() = default;
        
        void draw(const Camera &cam, openGL::Shader &shader);

        void moveToCam(const Camera &cam, glm::ivec3 min, glm::ivec3 max);

        glm::mat4 modelMatrix() const;

        void handleEvent(SDL_Event sdlEvent);

        inline glm::ivec3* getPointerPos() { return &_position; };
        inline void setPos(const glm::ivec3 p) { _position = p; };
        inline void setCameraReference(Camera &c) { _camReferenceForMove = &c; };
    };
}
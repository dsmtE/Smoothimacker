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
#include <unordered_map>

namespace world {

    class ControlPts {

    private:

        std::vector<glm::vec3> _cubeVertices;
        std::vector<glm::vec3> _controlCubesPos;
        glm::vec3 _color;

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO;
        openGL::VertexBuffer _instanceVBO;

    public:

        ControlPts(const glm::vec3 &color = glm::vec3(0, 0, 1));
        ~ControlPts() = default;
        
        void draw(const Camera &cam, openGL::Shader &shader);

        void addControlPts(const glm::vec3 &p);
        bool delControlPts(const glm::vec3 &p);
        void resetControlPts();

        inline std::vector<glm::vec3> getPts() { return _controlCubesPos; };

    };
}
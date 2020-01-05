#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#include "../openGL/ElementBuffer.hpp"
#include "../openGL/VertexBuffer.hpp"
#include "../openGL/VertexArray.hpp"

#include "../openGL/Shader.hpp"
#include "../world/Camera.hpp"
#include <unordered_map>

#define MAX_LIGHTS 128

namespace world {

    struct PointLight {    
        glm::vec3 pos;
        // attenuation
        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambientColor;
        glm::vec3 diffuseColor;
    };

    class PointsLights {

    private:

        std::vector<PointLight> _lights;

        std::vector<glm::vec3> _displayCubeVertices;
        std::vector<glm::uvec3> _displayCubeIndices;

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO; 
        openGL::ElementBuffer _EBO;
        openGL::VertexBuffer _lightPosVBO;

    public:

        PointsLights();
        ~PointsLights() = default;
        
        void draw(const Camera &cam, openGL::Shader &shader);

        void addPointLight(const glm::vec3 &p, const float &constant, const float &linear, const float &quadratic, const  glm::vec3 &ambientColor,const  glm::vec3 &diffuseColor);
        void resetPointLights();
        void delPointLight(const glm::vec3 &p);

        inline std::vector<PointLight> getLights() { return _lights; };

    };
}
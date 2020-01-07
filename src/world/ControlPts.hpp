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

    ///\brief class that is in charge of generating our control points and drawing them
    class ControlPts {

    private:

        std::vector<glm::vec3> _cubeVertices;
        std::vector<glm::vec3> _controlCubesPos;
        glm::vec3 _color;

        openGL::VertexArray _VAO;
        openGL::VertexBuffer _VBO;
        openGL::VertexBuffer _instanceVBO;

    public:
        ///\brief Construct a new Control Pts mananger object
        ///\param color color or our pts
        ControlPts(const glm::vec3 &color = glm::vec3(0, 0, 1));
        ~ControlPts() = default;
        
        ///\brief delegated function that manages the drawin of our controls pts
        ///
        ///\param cam 
        ///\param shader // shader used by our controls pts
        void draw(const Camera &cam, openGL::Shader &shader);

        ///\brief create a new pt
        ///\param p position where the point should be created
        void addControlPts(const glm::vec3 &p);

        ///\brief delete a control point 
        ///\param p position where the point should be deleted
        ///\return true if the pt has been suppressed.
        bool delControlPts(const glm::vec3 &p);
        void resetControlPts();

        inline std::vector<glm::vec3> getPts() { return _controlCubesPos; };

    };
}
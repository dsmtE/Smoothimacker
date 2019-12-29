#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

#include "../world/Camera.hpp"
#include "../imath/Ray.hpp"

namespace openGL {
    class Line {

	private:
        std::vector<glm::vec3> _endPts;

		openGL::VertexArray _VAO;
		openGL::VertexBuffer _VBO;
        openGL::Shader _shader;

        glm::vec3 _color;

	public:
    Line(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &color);
    Line(const imath::Ray &ray);
    void draw(const world::Camera &cam, openGL::Shader &shader);

    void setStart(const glm::vec3 &start);
    void setEnd(const glm::vec3 &end);

    };
}
#include "Line.hpp"

using namespace openGL;


Line::Line( const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &color ) : _color(color) {
    _endPts.push_back(start);
    _endPts.push_back(end);

    _VBO.setData<glm::vec3>(_endPts, GL_STATIC_DRAW);

    const GLuint vertexAttrib_Pos = 0;
	
	_VAO.bind();
	_VBO.bind();
	openGL::VertexBuffer::setVertexAttrib(vertexAttrib_Pos, 3, GL_FLOAT, sizeof(glm::vec3), 0);
	_VBO.unbind();
    _VAO.unbind();

}

void Line::draw(const world::Camera &cam, openGL::Shader &shader) {
    shader.bind();
    // send uniform to shader
    shader.setVec3f("lineColor", _color);
    shader.setMat4("MVPMatrix", cam.PVMatrix());

    _VAO.bind();
    glDrawArrays(GL_LINE_STRIP, 0, _endPts.size());
    _VAO.unbind();

    shader.unBind();
}

void Line::setStart(const glm::vec3 &start) {
    _endPts[0] = start;
    _VBO.setData<glm::vec3>(_endPts, GL_STATIC_DRAW);
}

void Line::setEnd(const glm::vec3 &end) {
    _endPts[1] = end;
    _VBO.setData<glm::vec3>(_endPts, GL_STATIC_DRAW);

}

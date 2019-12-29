#include "Chunk.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>

using namespace world;

// 	glm::vec3 c = glm::floor(_pos/glm::vec3(chunkSize));
//   glm::vec3 p = glm::mod(_pos, glm::vec3(chunkSize));

Chunk::Chunk(const unsigned int &depth, const glm::uvec3 &pos) :  _position(pos), _cubes(depth), needUpdateVBO(false) {
	// limit chunk size
    assert(depth <= 8); // limit chunk by (2^7)^3 = 2 097 152 elements 
	buildVAO();
}

bool Chunk::validCoordinate(const glm::uvec3 &pos) const {
	return glm::all(glm::lessThan(pos, glm::uvec3( _cubes.size() )));
}

void Chunk::buildVAO() {  
		const GLuint pos = 0;
		const GLuint type = 1;
		const GLuint faceMask = 2;
		_VAO.bind();
		_VBO.bind();
		openGL::VertexBuffer::setVertexAttribInteger(pos, 3, GL_UNSIGNED_INT, sizeof(CubeVertex), offsetof(CubeVertex, pos));
		openGL::VertexBuffer::setVertexAttribInteger(type, 1, GL_UNSIGNED_BYTE, sizeof(CubeVertex), offsetof(CubeVertex, type));
		openGL::VertexBuffer::setVertexAttribInteger(faceMask, 1, GL_UNSIGNED_BYTE, sizeof(CubeVertex), offsetof(CubeVertex, faceMask));
		_VBO.unbind();
		_VAO.unbind();
}

void Chunk::setVBOdata() {
	_VBO.setData<CubeVertex>(_cubes.vector(), GL_STREAM_DRAW);
}

// glm::uvec3 Chunk::getPositionFromOctreeSubIndex(const std::vector<uint8_t> &OSubId) const {
// 	glm::uvec3 pos(0, 0, 0);
// 	for (unsigned int i = 0; i < OSubId.size(); i++) {
// 		pos += mortonIdToPos(OSubId[i]) * ( unsigned int(size()) >> (i+1) );
// 	}
// 	return pos;
// }

void Chunk::draw(const world::Camera &c,  openGL::Shader &s) {
	if (needUpdateVBO) {
		setVBOdata(); // update data in VBO
		needUpdateVBO = false;
	}

	if (_cubes.vector().size() > 0) {
		s.bind();

		// s.setInt("chunkSize", size());
		const glm::mat4 MVMatrix = c.viewMatrix() * getModelMatrix();
		s.setMat4("MVPMatrix", c.projectionMatrix() * MVMatrix);
        // TODO add support of normal matrix for light
		// s.setMat3("NormalMatrix", glm::inverseTranspose(glm::mat3(MVMatrix)));

		_VAO.bind();
		glDrawArrays(GL_POINTS, 0, _cubes.vector().size());
		_VAO.unbind();

		s.unBind();
	}
}


const uint8_t Chunk::getType(const glm::uvec3 &pos) {
	return _cubes.getType(pos);
}

bool Chunk::delAt(const glm::uvec3 &pos) {
	if ( _cubes.delAt(pos) ) {
		// need update local our VBO data insted of update all with  setVBOdata()
		needUpdateVBO = true;
		return true;
	}
	return false;
}

bool Chunk::setType(const glm::uvec3 &pos, const uint8_t &type, const bool updateFaceMask) {
	if ( _cubes.setType(pos, type, updateFaceMask) ) {
		// need update local our VBO data insted of update all with setVBOdata()
		needUpdateVBO = true;
		return true;
	}
	return false;
}

glm::mat4 Chunk::getModelMatrix() const {
	glm::mat4 m(1.0f);
	m = glm::translate(m, glm::vec3(_position * unsigned int( _cubes.size() )));
	return m;
}

unsigned int  Chunk::size() const {
	return _cubes.size();
};

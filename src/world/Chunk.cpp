#include "Chunk.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>

using namespace world;

Chunk::Chunk(const unsigned int &depth, const glm::uvec3 &pos) : _size(1 << depth), _position(pos), _cubesType(depth), needRemesh(false) {
	// limit chunk size
    assert(depth <= 8); // limit chunk by (2^8)^3 = 16777216 elements 
	buildVAO();
}

bool Chunk::validCoordinate(const glm::uvec3 &pos) const {
	return pos.x < _size && pos.y < _size && pos.z < _size;
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
	if (hasMesh()) {
		_VBO.setData<CubeVertex>(_mesh, GL_STREAM_DRAW);
	}
}

void Chunk::updateMesh(const glm::uvec3 &pos, const uint8_t &type) {
	// TODO
	//  update our mesh
	// TODO get adjacent type from octree and update faceMask
	// update adjacent faceMask 
	//  -> & VBO using setSubData
}

void Chunk::buildMesh() {
	// TODO
	_mesh.clear();
}
/*
std::vector<std::pair<Direction, CubeVertex*>> Chunk::getAdjacentsCube(const uint16_t &id) {	
	std::vector<std::pair<Direction, CubeVertex*>> existingCubes;
	existingCubes.reserve(6); // max 6 elements, reserve allow us to use emplace_back

	glm::uvec3 coordo = indexToCoord(id);

	// up 
	if(int(coordo.y) + 1 < _size) {
		existingCubes.emplace_back( std::make_pair(Up, _cubes[id + uint16_t(_size) * uint16_t(_size)]) );
	}
	// down 
	if(int(coordo.y) - 1 >= 0) {
		existingCubes.emplace_back( std::make_pair(Down, _cubes[id - uint16_t(_size) * uint16_t(_size)]) );
	}
	// left 
	if(int(coordo.x) - 1 >= 0) {
		existingCubes.emplace_back( std::make_pair(Left, _cubes[id - uint16_t(1)]) );
	}
	// right
	if(int(coordo.x) + 1 < _size ) {
		existingCubes.emplace_back( std::make_pair(Right, _cubes[id + uint16_t(1)]) );
	}
	// front
	if(int(coordo.z) + 1 < _size ) {
		existingCubes.emplace_back( std::make_pair(Front, _cubes[id + uint16_t(_size)]) );
	}
	// back
	if(int(coordo.z) - 1 >= 0) {
		existingCubes.emplace_back( std::make_pair(Back, _cubes[id - uint16_t(_size)]) );
	}
	return existingCubes;
}

void Chunk::updateCubeMaskAndAdjacents(const uint16_t &id) {
	CubeVertex* c = _cubes[id];
    if (c != nullptr) {
        c->faceMask = 0x3F; // makes all sides visible (0b00111111)
    }
	
	std::vector<std::pair<Direction, CubeVertex*>> cubes = getAdjacentsCube(id);

	for(size_t i = 0; i != cubes.size(); i++) { // for each adjacents Cube
		switch (cubes[i].first) {
			case Up:
				if (cubes[i].second != nullptr) {
					if (c != nullptr) { c->faceMask -= 0x01; }
                    // TODO use mask directly insted of recurs
					updateCubeMask( (cubes[i].second)->indexInChunk );
				}
				break;
			case Down:
				if (cubes[i].second != nullptr) {
					if (c != nullptr) { c->faceMask -= 0x02; }
					updateCubeMask((cubes[i].second)->indexInChunk);
				}
				break;
			case Left:
				if (cubes[i].second != nullptr) {
					if (c != nullptr) { c->faceMask -= 0x04; }
					updateCubeMask((cubes[i].second)->indexInChunk);
				}
				break;
			case Right:
				if (cubes[i].second != nullptr) {
					if (c != nullptr) { c->faceMask -= 0x08; }
					updateCubeMask((cubes[i].second)->indexInChunk);
				}
				break;
			case Front:
				if (cubes[i].second != nullptr) {
					if (c != nullptr) { c->faceMask -= 0x10; }
					updateCubeMask((cubes[i].second)->indexInChunk);
				}
				break;
			case Back:
				if (cubes[i].second != nullptr) {
					if (c != nullptr) { c->faceMask -= 0x20; }
					updateCubeMask((cubes[i].second)->indexInChunk);
				}
				break;
			
			default:
			    std::cerr << "error: unknown direction" << std::endl;
				break;
		}
	}
}

void Chunk::updateCubeMask(const uint16_t &id) {
	CubeVertex* c = _cubes[id];
    assert(c != nullptr); // trying to update the mask of a non-existent cube.
	c->faceMask = 0b00111111;
	std::vector<std::pair<Direction, CubeVertex*>> cubes = getAdjacentsCube(id);

	for(size_t i = 0; i != cubes.size(); i++) {
		switch (cubes[i].first) {
			case Up:
				if (cubes[i].second != nullptr) {
					c->faceMask -= 0b00000001;
				}
				break;
			case Down:
				if (cubes[i].second != nullptr) {
					c->faceMask -= 0b00000010;
				}
				break;
			case Left:
				if (cubes[i].second != nullptr) {
					c->faceMask -= 0b00000100;
				}
				break;
			case Right:
				if (cubes[i].second != nullptr) {
					c->faceMask -= 0b00001000;
				}
				break;
			case Front:
				if (cubes[i].second != nullptr) {
					c->faceMask -= 0b00010000;
				}
				break;
			case Back:
				if (cubes[i].second != nullptr) {
					c->faceMask -= 0b00100000;
				}
				break;			
			default:
			    std::cerr << "error: unknown direction" << std::endl;
				break;
		}
	}
}
*/

//        (Z)
//         ^
//         |
//         .---->(X)
//        /
//   (Y) v 
glm::uvec3 Chunk::indexToCoord(const uint16_t &id) const {
	return glm::uvec3(id % _size, id / (_size * _size), (id / _size) % _size);
}

uint16_t Chunk::coordToIndex(const glm::uvec3 &pos) const {
    return pos.x + pos.y * _size + pos.z * _size * _size;
}

void Chunk::draw(const world::Camera &c, const int& screenWidth, const int& screenHeigh,  openGL::Shader &s) {
    if(needRemesh) {
        buildMesh(); // update Mesh
	    setVBOdata(); // update data in VBO
        needRemesh = false;
    }

	if (hasMesh()) {
		s.bind();

		s.setInt("chunkSize", size());
		const glm::mat4 MVMatrix = c.viewMatrix() * getModelMatrix();
		s.setMat4("MVPMatrix", c.projectionMatrix(screenWidth, screenHeigh) * MVMatrix);
        // TODO add supoprt of normal matrix for light
		// s.setMat3("NormalMatrix", glm::inverseTranspose(glm::mat3(MVMatrix)));

		_VAO.bind();
		glDrawArrays(GL_POINTS, 0, _mesh.size());
		_VAO.unbind();

		s.unBind();
	}
}

uint8_t* Chunk::getCubeType(const glm::uvec3 &pos) const {
	return _cubesType.getValue(pos);
}

void Chunk::setCube(const glm::uvec3 &pos, const uint8_t &type) {
	uint8_t* actualType = _cubesType.getValue(pos);
	if ( _cubesType.setValue(pos, type)) { // if this modify our Octree
		if (actualType != nullptr) {
			updateMesh(pos, type); // just update our mesh
		}else { // else if nullptr have been set we need to reconstruct mesh
			needRemesh = true;
		}
		
	}
}

void Chunk::delCube(const glm::uvec3 &pos) {
	if ( _cubesType.delValue(pos) ) {
    	needRemesh = true; // if one value was deleted, we need to reconstruct mesh
	}
}

glm::mat4 Chunk::getModelMatrix() const {
	glm::mat4 m(1.0f);
	m = glm::translate(m, glm::vec3(_position * unsigned int(_size)));
	return m;
}

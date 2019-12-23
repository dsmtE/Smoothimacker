#include "Chunk.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>

using namespace world;

// 	glm::vec3 c = glm::floor(_pos/glm::vec3(chunkSize));
//   glm::vec3 p = glm::mod(_pos, glm::vec3(chunkSize));

Chunk::Chunk(const unsigned int &depth, const glm::uvec3 &pos) : _size(1 << depth), _position(pos), _cubesType(depth), needRemesh(false) {
	// limit chunk size
    assert(depth <= 8); // limit chunk by (2^8)^3 = 16777216 elements 
	buildVAO();
}

bool Chunk::validCoordinate(const glm::uvec3 &pos) const {
	return glm::all(glm::lessThan(pos, glm::uvec3( _size )));
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

glm::uvec3 Chunk::mortonIdToPos(const uint8_t &id) const {
	return glm::uvec3(id % 2, id / 4, (id / 2) % 2);
}

glm::uvec3 Chunk::getPositionFromOctreeSubIndex(const std::vector<uint8_t> &OSubId) const {
	glm::uvec3 pos(0, 0, 0);
	for (unsigned int i = 0; i < OSubId.size(); i++) {
		pos += mortonIdToPos(OSubId[i]) * ( unsigned int(size()) >> (i+1) );
	}
	return pos;
}

void Chunk::buildMesh() {
	_mesh.clear();
	_mesh.reserve(1 << _cubesType.size());

	//  iterate through Octree and add value
	Octree<uint8_t>* workOctree = &_cubesType; // begin from root Octree

	std::vector<uint8_t> OctreeSubIndex; // store subIndex checked at each subLevel
	OctreeSubIndex.reserve(_cubesType.depth());
	OctreeSubIndex.push_back(0);

	while (OctreeSubIndex[0] < 8) { // until we visit all subOctree
		if( workOctree->isLeaf() ) {
			uint8_t* val = workOctree->getValue();
			if (val != nullptr) {
				_mesh.push_back(CubeVertex( getPositionFromOctreeSubIndex(OctreeSubIndex), *val, 255 ) );
			}
		}
		// next subIndex
		OctreeSubIndex.back()++;
		while (OctreeSubIndex.back() >= 8 && OctreeSubIndex[0] < 8) {
			OctreeSubIndex.pop_back();
			OctreeSubIndex.back()++;
		}
	}
	// mesh done with all non null value of subOctree
}

/*

void Chunk::updateMesh(const glm::uvec3 &pos, const uint8_t &type) {
	// TODO
	//  update our mesh
	// TODO get adjacent type from octree and update faceMask
	// update adjacent faceMask 
	//  -> & VBO using setSubData

	std::vector<std::pair<Direction, CubeVertex*>> cubes = getAdjacentsCube(pos);
	CubeVertex* c = nullptr;
	for (CubeVertex c : _mesh) {
		if (c.pos == pos)
	}

	
    if (c != nullptr) {
        c->faceMask = 0x3F; // makes all sides visible (0b00111111)
    }
}

std::vector<std::pair<Direction, uint8_t*>> Chunk::getAdjacentsCube(const glm::uvec3 &pos) {	
	std::vector<std::pair<Direction, uint8_t*>> existingCubes;
	existingCubes.reserve(6); // max 6 elements, reserve allow us to use emplace_back

	// up 
	if(int(pos.y) + 1 < _size) {
		existingCubes.emplace_back(Up, _cubesType.getValue(pos + glm::uvec3(0, 1, 0)));
	}
	// down 
	if(int(pos.y) - 1 >= 0) {
		existingCubes.emplace_back(Down, _cubesType.getValue(pos + glm::uvec3(0, -1, 0)));
	}
	// left 
	if(int(pos.x) - 1 >= 0) {
		existingCubes.emplace_back(Left,  _cubesType.getValue(pos + glm::uvec3(1, 0, 0)));
	}
	// right
	if(int(pos.x) + 1 < _size ) {
		existingCubes.emplace_back(Right,  _cubesType.getValue(pos + glm::uvec3(-1, 0, 0)));
	}
	// front
	if(int(pos.z) + 1 < _size ) {
		existingCubes.emplace_back(Front,  _cubesType.getValue(pos + glm::uvec3(0, 0, 1)));
	}
	// back
	if(int(pos.z) - 1 >= 0) {
		existingCubes.emplace_back(Back,  _cubesType.getValue(pos + glm::uvec3(0, 0, -1)));
	}
	return existingCubes;
}

void Chunk::updateCubeMaskAndAdjacents(const uint16_t &id) {
	CubeVertex* c = _cubes[id];
    if (c != nullptr) {
        c->faceMask = 0x3F; // makes all sides visible (0b00111111)
    }
	
	

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
        // TODO add support of normal matrix for light
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
	if ( _cubesType.setValue(pos, type)) { // if this modify our Octree
		// updateMesh(pos, type); // update our mesh
		// without remesh all
		needRemesh = true; // for debug
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

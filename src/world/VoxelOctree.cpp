#include "VoxelOctree.hpp"

#include <string>
#include <iostream>

using namespace world;

VoxelOctree::VoxelOctree(const unsigned int &depth) : ListLinkedOctree<CubeVertex>(depth) {
}

glm::uvec3 VoxelOctree::mortonIdToPos(const uint8_t &id) const {
	return glm::uvec3(id % 2, id / 4, (id / 2) % 2);
}

glm::uvec3 VoxelOctree::getPositionFromOctreeSubIndex(const std::vector<uint8_t> &OSubId) const {
	glm::uvec3 pos(0, 0, 0);
	for (unsigned int i = 0; i < OSubId.size(); i++) {
		pos += mortonIdToPos(OSubId[i]) * ( unsigned int(size()) >> (i+1) );
	}
	return pos;
}

std::vector<std::pair<Direction, CubeVertex*>> VoxelOctree::getAdjacentsCubes(const glm::uvec3 &pos) {	
	std::vector<std::pair<Direction, CubeVertex*>> existingCubes;
	existingCubes.reserve(6); // max 6 elements, reserve allow us to use emplace_back

	// up 
	if(int(pos.y) + 1 < int(size())) {
        try {
            existingCubes.push_back( std::make_pair(Up, &getValue(glm::uvec3(pos.x, pos.y+1, pos.z)) ) );
        }catch(std::string const str) {} // catch nothing
	}
	// down 
	if(int(pos.y) - 1 >= 0) {
		try { 
            existingCubes.push_back( std::make_pair(Down, &getValue(glm::uvec3(pos.x, pos.y-1, pos.z)) ) );
        }catch(std::string const str) {}
	}
	// left 
	if(int(pos.x) - 1 >= 0) {
		try { 
            existingCubes.push_back( std::make_pair(Left,  &getValue(glm::uvec3(pos.x-1, pos.y, pos.z)) ) );
        }catch(std::string const str) {}
	}
	// right
	if(int(pos.x) + 1 < int(size()) ) {
		try { 
            existingCubes.push_back( std::make_pair(Right,  &getValue(glm::uvec3(pos.x+1, pos.y, pos.z)) ) );
        }catch(std::string const str) {}
	}
	// front
	if(int(pos.z) + 1 < int(size()) ) {
		try { 
            existingCubes.push_back( std::make_pair(Front,  &getValue(glm::uvec3(pos.x, pos.y, pos.z+1)) ) );
        }catch(std::string const str) {}
	}
	// back
	if(int(pos.z) - 1 >= 0) {
		try { 
            existingCubes.push_back( std::make_pair(Back,  &getValue(glm::uvec3(pos.x, pos.y, pos.z-1)) ) );
        }catch(std::string const str) {}
	}
	return existingCubes;
}

uint8_t VoxelOctree::getType(const glm::uvec3 &pos) {
    return getValue(pos).type;
}

bool VoxelOctree::delAt(const glm::uvec3 &pos) {
    
    if( delValue(pos) ) {

        // cVertex->faceMask = 0x3F; // makes all sides visible by default (0b00111111)

        std::vector<std::pair<Direction, CubeVertex*>> adjCubes = getAdjacentsCubes(pos);

        for(auto adjC : adjCubes) { // for each adjacents Cubes
            switch (adjC.first) {
                case Up:
                    (adjC.second)->faceMask -= 0x02;
                    break;
                case Down:
                    (adjC.second)->faceMask -= 0x01;
                    break;

                case Left:
                    (adjC.second)->faceMask -= 0x08;
                    break;

                case Right:
                    (adjC.second)->faceMask -= 0x04;
                    break;

                case Front:
                    (adjC.second)->faceMask -= 0x20;
                    break;

                case Back:
                    (adjC.second)->faceMask -= 0x10;
                    break;

                default:
                    std::cerr << "error: unknown direction" << std::endl;
                    break;
            }
        }
        return true;
    }else {
        return false;
    }
}

bool VoxelOctree::setType(const glm::uvec3 &pos, const uint8_t &type) {
    unsigned int** idPtr = _octree.getOrCreateValue(pos);
	if(*idPtr == nullptr) {
		_data.emplace_back(pos, type, 0x3F); // alocate new value (0x3F (0b00111111) makes all sides visible by default)
        assert(_data.size() > 0);
		*idPtr = new unsigned int(_data.size()-1); // create new in octree (new using reference)(destroyed in octree destructor)
	} else {
        if(_data[**idPtr].type != type) {
		    _data[**idPtr].type = type; // change value using copy constructor (reference of vector)
        } else {
            return false;
        }
	}
    std::vector<std::pair<Direction, CubeVertex*>> adjCubes = getAdjacentsCubes(pos);

    for(auto adjC : adjCubes) { // for each adjacents Cubes
        switch (adjC.first) {
            case Up:
                _data[**idPtr].faceMask -= 0x01; // update mask at this position
                (adjC.second)->faceMask -= 0x02; // update mask on ajdacent position
                break;
            case Down:
                _data[**idPtr].faceMask -= 0x02;
                (adjC.second)->faceMask -= 0x01;
                break;

            case Left:
                _data[**idPtr].faceMask -= 0x04;
                (adjC.second)->faceMask -= 0x08;
                break;

            case Right:
                _data[**idPtr].faceMask -= 0x08;
                (adjC.second)->faceMask -= 0x04;
                break;

            case Front:
                _data[**idPtr].faceMask -= 0x10;
                (adjC.second)->faceMask -= 0x20;
                break;

            case Back:
                _data[**idPtr].faceMask -= 0x20;
                (adjC.second)->faceMask -= 0x10;
                break;

            default:
                std::cerr << "error: unknown direction" << std::endl;
                break;
        }
    }
    return true;
}
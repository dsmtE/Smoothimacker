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
        CubeVertex* c = getPtrValue(glm::uvec3(pos.x, pos.y+1, pos.z)); 
        if( c != nullptr ) {
            existingCubes.push_back( std::make_pair(Up, c ) );
        }
	}
	// down 
	if(int(pos.y) - 1 >= 0) {
		CubeVertex* c = getPtrValue(glm::uvec3(pos.x, pos.y-1, pos.z)); 
        if( c != nullptr ) { 
            existingCubes.push_back( std::make_pair(Down, c ) );
        }
	}
	// left 
	if(int(pos.x) - 1 >= 0) {
		CubeVertex* c = getPtrValue(glm::uvec3(pos.x-1, pos.y, pos.z)); 
        if( c != nullptr ) { 
            existingCubes.push_back( std::make_pair(Left, c ) );
        }
	}
	// right
	if(int(pos.x) + 1 < int(size()) ) {
		CubeVertex* c = getPtrValue(glm::uvec3(pos.x+1, pos.y, pos.z)); 
        if( c != nullptr ) { 
            existingCubes.push_back( std::make_pair(Right, c ) );
        }
	}
	// front
	if(int(pos.z) + 1 < int(size()) ) {
		CubeVertex* c = getPtrValue(glm::uvec3(pos.x, pos.y, pos.z+1)); 
        if( c != nullptr ) { 
            existingCubes.push_back( std::make_pair(Front, c ) );
        }
	}
	// back
	if(int(pos.z) - 1 >= 0) {
		CubeVertex* c = getPtrValue(glm::uvec3(pos.x, pos.y, pos.z-1)); 
        if( c != nullptr ) { 
            existingCubes.push_back( std::make_pair(Back, c ) );
        }
	}
	return existingCubes;
}

uint8_t VoxelOctree::getType(const glm::uvec3 &pos) {
    return getValue(pos).type;
}

bool VoxelOctree::delAt(const glm::uvec3 &pos) {   
    if( delValue(pos) ) {
        std::vector<std::pair<Direction, CubeVertex*>> adjCubes = getAdjacentsCubes(pos);

        for(auto adjC : adjCubes) { // for each adjacents Cubes
            switch (adjC.first) {
                case Up:
                    (adjC.second)->faceMask |=0x02;
                    break;

                case Down:
                    (adjC.second)->faceMask |=0x01;
                    break;

                case Left:
                    (adjC.second)->faceMask |=0x08;
                    break;

                case Right:
                    (adjC.second)->faceMask |=0x04;
                    break;

                case Front:
                    (adjC.second)->faceMask |=0x20;
                    break;

                case Back:
                    (adjC.second)->faceMask |=0x10;
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

bool VoxelOctree::setType(const glm::uvec3 &pos, const uint8_t &type, const bool updateFaceMask) {
    unsigned int** idPtr = _octree.getOrCreateValue(pos);
	if(*idPtr == nullptr) {
		_data.emplace_back(pos, type, 0x3F); // alocate new value (0x3F (0b00111111) makes all sides visible by default)
        assert(_data.size() > 0);
		*idPtr = new unsigned int(_data.size()-1); // create new in octree (new using reference)(destroyed in octree destructor)
        _valueOctreePtr.push_back(idPtr); // save ptr to ptr to id in octree coresponding to value in data vector
	} else {
        if(_data[**idPtr].type != type) {
		    _data[**idPtr].type = type; // change value using copy constructor (reference of vector)
        } else {
            return false;
        }
	}

    if(updateFaceMask) {
        std::vector<std::pair<Direction, CubeVertex*>> adjCubes = getAdjacentsCubes(pos);
        CubeVertex &cv = _data[**idPtr];
        for(auto &adjC : adjCubes) { // for each adjacents Cubes
            switch (adjC.first) {
                case Up:
                    cv.faceMask &= 0x3F-0x01; // update mask at this position
                    (adjC.second)->faceMask &= 0x3F-0x02; // update mask on ajdacent position
                    break;
                case Down:
                    cv.faceMask &= 0x3F-0x02;
                    (adjC.second)->faceMask &= 0x3F-0x01;
                    break;
                // TODO fix left mask not work on set new type
                case Left:
                    cv.faceMask &= 0x3F-0x04;
                    (adjC.second)->faceMask &= 0x3F-0x08;
                    break;

                case Right:
                    cv.faceMask &= 0x3F-0x08;
                    (adjC.second)->faceMask &= 0x3F-0x04;
                    break;

                case Front:
                    cv.faceMask &= 0x3F-0x10;
                    (adjC.second)->faceMask &= 0x3F-0x20;
                    break;

                case Back:
                    cv.faceMask &= 0x3F-0x20;
                    (adjC.second)->faceMask &= 0x3F-0x10;
                    break;

                default:
                    std::cerr << "error: unknown direction" << std::endl;
                    break;
            }
        }
    }
    return true;
}

void VoxelOctree::updateAllFaceMask() {
    for(CubeVertex &cv : _data) {
        std::vector<std::pair<Direction, CubeVertex*>> adjCubes = getAdjacentsCubes(cv.pos);
        cv.faceMask = 0x3F;
        for(auto adjC : adjCubes) { // for each adjacents Cubes
            switch (adjC.first) {
                case Up:
                    cv.faceMask -= 0x01;
                    break;

                case Down:
                    cv.faceMask -= 0x02;
                    break;

                case Left:
                    cv.faceMask -= 0x04;
                    break;

                case Right:
                    cv.faceMask -= 0x08;
                    break;

                case Front:
                    cv.faceMask -= 0x10;
                    break;

                case Back:
                    cv.faceMask -= 0x20;
                    break;

                default:
                    std::cerr << "error: unknown direction" << std::endl;
                    break;
            }
        }
    }
}
#include "VoxelOctree.hpp"

#include <string>
#include <iostream>
#include <fstream> // for files

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

glm::vec3 VoxelOctree::getColor(const glm::uvec3 &pos) {
    return getValue(pos).color;
}

glm::vec3* VoxelOctree::getColorPtr(const glm::uvec3 &pos) {
    CubeVertex* val = getPtrValue(pos);
    if(val != nullptr) {
        return &(val->color);
    }
    return nullptr;
}

bool VoxelOctree::delAt(const glm::uvec3 &pos, const bool updateFaceMask) {   
    if( delValue(pos) ) {
        std::vector<std::pair<Direction, CubeVertex*>> adjCubes = getAdjacentsCubes(pos);

        if (updateFaceMask) {
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
        }
        return true;
    }else {
        return false;
    }
}

bool VoxelOctree::setColor(const glm::uvec3 &pos, const glm::vec3 &color, const bool updateFaceMask) {
    unsigned int** idPtr = _octree.getOrCreateValue(pos);
	if(*idPtr == nullptr) {
		_data.emplace_back(pos, color, 0x3F); // alocate new value (0x3F (0b00111111) makes all sides visible by default)
        assert(_data.size() > 0);
		*idPtr = new unsigned int(_data.size()-1); // create new in octree (new using reference)(destroyed in octree destructor)
        _valueOctreePtr.push_back(idPtr); // save ptr to ptr to id in octree coresponding to value in data vector
	} else {
        if(_data[**idPtr].color != color) {
		    _data[**idPtr].color = color; // change value using copy constructor (reference of vector)
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


bool VoxelOctree::saveVoxelData(const char* filePath) const {
	std::ofstream file(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if(!file.is_open())
		return false;

    int floatSize = sizeof(float);
    int uIntSize = sizeof(unsigned int);

    unsigned int size = _octree.size();
	file.write((char*)& size, uIntSize);

	for(const CubeVertex &cv : _data) {
        // Position
		file.write((char*)& cv.pos.r, uIntSize);
		file.write((char*)& cv.pos.g, uIntSize);
		file.write((char*)& cv.pos.b, uIntSize);
		// Colour
		file.write((char*)& cv.color.x, floatSize);
		file.write((char*)& cv.color.y, floatSize);
		file.write((char*)& cv.color.z, floatSize);
	}
	file.close();
	return true;
}

DataHandler VoxelOctree::loadVoxelData(const char* filePath) {
	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	if(!file.is_open()) {
		return DataHandler::ERROR_NO_FILE;
    }
    // reset our voxelOctree
    reset();

    unsigned int* sizeBuffer = new unsigned int[1] { 0 };
    
    file.read((char*)sizeBuffer, sizeof(unsigned int));
	if(sizeBuffer[0] > size()) {
		file.close();
		delete[] sizeBuffer;
        std::cerr << "we can't load a file containing a chunck larger than the one we have. " << std::endl;
		return DataHandler::ERROR_CORRUPTED;
	}
    std::cout << sizeBuffer[0]<< std::endl;
    delete[] sizeBuffer;

	unsigned int* posBuffer = new unsigned int[3] { 0 };
    float* colorBuffer = new float[3] { 0.0f };

	while(file.peek() != EOF) {
        file.read((char*)posBuffer, 3 * sizeof(unsigned int));
		file.read((char*)colorBuffer, 3 *  sizeof(float));
		if((file.rdstate() & std::ifstream::failbit) != 0 || (file.rdstate() & std::ifstream::badbit)) { 
			file.close(); 
			delete[] colorBuffer;
            delete[] posBuffer;
			return DataHandler::ERROR_CORRUPTED; 
		}
        setColor(glm::uvec3(unsigned int(posBuffer[0]), unsigned int(posBuffer[1]), unsigned int(posBuffer[2])), glm::vec3(colorBuffer[0], colorBuffer[1], colorBuffer[2]));
	}
    updateAllFaceMask();
	delete[] colorBuffer;
    delete[] posBuffer;
	return DataHandler::SUCCESS;
}
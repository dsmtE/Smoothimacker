#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <vector>

#include "../openGL/VertexBuffer.hpp"
#include "../openGL/VertexArray.hpp"
#include "../openGL/Shader.hpp"

#include "Camera.hpp"
#include "Octree.hpp"
// #include "../imath/util.hpp"

namespace world {

	struct CubeVertex {
		glm::uvec3 pos;
		uint8_t type;
		uint8_t faceMask;

		CubeVertex() = default;
		CubeVertex(const glm::uvec3& p, const uint8_t& t, const uint8_t& m) : pos(p), type(t), faceMask(m) {}
	};

	enum Direction { Up, Down, Left, Right, Front, Back };

	class Chunk {

	private:
        //---------- Attributes ----------//

		Octree<uint8_t> _cubesType; // octree of type
		std::vector<CubeVertex> _mesh; // send it to GPU with only used CubeVertex in cubes (copy of original Cubes in _cubes)

        bool needRemesh;
		glm::uvec3 _position; // position of chunk in world
		uint8_t _size;

		openGL::VertexArray _VAO;
		openGL::VertexBuffer _VBO;

        //---------- Private Methods ----------//

        // VAO build called in constructor
        void buildVAO();

		//privates methods used in mesh Reconstruction // TODO
		glm::uvec3 mortonIdToPos(const uint8_t &id) const;
		glm::uvec3 getPositionFromOctreeSubIndex(const std::vector<uint8_t> &OSubId) const;
		std::vector<std::pair<Direction, uint8_t*>> getAdjacentsCube(const glm::uvec3 &pos);
		void updateCubeMask(const uint16_t &id);
		void updateCubeMaskAndAdjacents(const uint16_t& id);

		void updateMesh(const glm::uvec3 &pos, const uint8_t &type);
		void buildMesh();
		void setVBOdata();

        // check if the coordinates are within the limits
        bool validCoordinate(const glm::uvec3 &pos) const;

		// index <=> coordinate convertion functions  (deprecated)
		glm::uvec3 indexToCoord(const uint16_t &id) const;
        uint16_t coordToIndex(const glm::uvec3 &pos) const;

	public:

		Chunk(const unsigned int &depth = 0, const glm::uvec3 &pos = glm::uvec3(0,0,0));
		~Chunk() = default;

        //---------- Methods ----------//

		void draw(const Camera &c, const int& screenWidth, const int& screenHeigh, openGL::Shader &s);

        // cubes setters and getters
        uint8_t* getCubeType(const glm::uvec3 &pos) const;
		void setCube(const glm::uvec3 &pos, const uint8_t &type);
        void delCube(const glm::uvec3 &pos);

        //---------- Getters & Setters ----------//

		glm::mat4 getModelMatrix() const;

        inline uint16_t capacity() const { return _size * _size * _size; };
		inline uint8_t size() const { return _size; };
		inline bool hasMesh() const { return _mesh.size() != 0; };
        inline void setPosition(const glm::uvec3& pos) { _position = pos; }

        void updateAllCubesMask();

        //---------- Compatibility fonctions ----------//
		/*
        inline uint16_t coordToIndex(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {coordToIndex(glm::uvec3(x, y, z));}
        inline CubeVertex* getCube(const unsigned int &x, const unsigned int &y, const unsigned int &z) { return getCube(glm::uvec3(x, y, z));}
		inline void setCube(const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &type) {setCube(glm::uvec3(x, y, z), type); }
        inline bool validCoordinate(const unsigned int& x, const unsigned int& y, const unsigned int& z) const {validCoordinate(glm::uvec3(x, y, z));}
		*/
	};
}
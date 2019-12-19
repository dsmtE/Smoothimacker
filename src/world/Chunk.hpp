#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <vector>

#include "../openGL/VertexBuffer.hpp"
#include "../openGL/VertexArray.hpp"
#include "../openGL/Shader.hpp"

#include "Camera.hpp"


namespace world {

	struct CubeVertex {
		uint16_t indexInChunk; // max 
		uint8_t type;
		uint8_t faceMask;

		CubeVertex() = default;
		CubeVertex(const uint16_t& i, const uint8_t& t, const uint8_t& m) : indexInChunk(i), type(t), faceMask(m) {}
	};

	enum Direction { Up, Down, Left, Right, Front, Back };

	class Chunk {

	private:
        //---------- Attributes ----------//

		std::vector<CubeVertex*> _cubes; // array of fixed size of pointer CubeVertex 
		std::vector<CubeVertex> _mesh; // send it to GPU with only used CubeVertex in cubes (copy of original Cubes in _cubes)

        bool needRemesh;
		glm::uvec3 _position; // position of chunk in world
		uint8_t _size;

		openGL::VertexArray _VAO;
		openGL::VertexBuffer _VBO;

        //---------- Private Methods ----------//

        // VAO build called in constructor
        void buildVAO();

		std::vector<std::pair<Direction, CubeVertex*>> getAdjacentsCube(const uint16_t& id);
		void updateCubeMask(const uint16_t &id);
		void updateCubeMaskAndAdjacents(const uint16_t& id);

        // check if the coordinates are within the limits
        bool validCoordinate(const glm::uvec3 &pos) const;

	public:

		Chunk(const uint8_t &size, const glm::uvec3 &pos = glm::uvec3(0,0,0));
		~Chunk();

        //---------- Methods ----------//

		void draw(const Camera &c, const int& screenWidth, const int& screenHeigh, openGL::Shader &s);
		
		void buildMesh();
		void setVBOdata();

        // index <=> coordinate convertion functions
		glm::uvec3 indexToCoord(const uint16_t &id) const;
        uint16_t coordToIndex(const glm::uvec3 &pos) const;

        // cubes setters and getters
        CubeVertex* getCube(const glm::uvec3 &pos);
		void setCube(const glm::uvec3 &pos, const uint8_t &type);
        void delCube(const glm::uvec3 &pos);

        //---------- Getters & Setters ----------//

		glm::mat4 getModelMatrix() const;

        inline uint16_t capacity() const { return _size * _size * _size; };
		inline uint8_t size() const { return _size; };
		inline bool hasMesh() const { return _mesh.size() != 0; };
        inline void setPosition(const glm::uvec3& pos) { _position = pos; }

        void updateAllCubesMask();

        //---------- Compatibility fonction ----------//
        inline uint16_t coordToIndex(const uint8_t &x, const uint8_t &y, const uint8_t &z) const {coordToIndex(glm::uvec3(x, y, z));}
        inline CubeVertex* getCube(const uint8_t &x, const uint8_t &y, const uint8_t &z) { return getCube(glm::uvec3(x, y, z));}
		inline void setCube(const uint8_t &x, const uint8_t &y, const uint8_t &z, const uint8_t &type) {setCube(glm::uvec3(x, y, z), type); }
        inline bool validCoordinate(const uint8_t& x, const uint8_t& y, const uint8_t& z) const {validCoordinate(glm::uvec3(x, y, z));}
	};
}
#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <vector>

#include "../openGL/VertexBuffer.hpp"
#include "../openGL/VertexArray.hpp"
#include "../openGL/Shader.hpp"

#include "Camera.hpp"
#include "VoxelOctree.hpp"
// #include "../imath/util.hpp"

namespace world {

	class Chunk {

	private:
        //---------- Attributes ----------//

		VoxelOctree _cubes; // octree of cubes

		glm::uvec3 _position; // position of chunk in world

		openGL::VertexArray _VAO;
		openGL::VertexBuffer _VBO;

		bool needUpdateVBO;

        //---------- Private Methods ----------//

        // VAO build called in constructor
        void buildVAO();
		void setVBOdata();

        // check if the coordinates are within the limits
        bool validCoordinate(const glm::uvec3 &pos) const;

	public:

		Chunk(const unsigned int &depth = 0, const glm::uvec3 &pos = glm::uvec3(0,0,0));
		~Chunk() = default;

        //---------- Methods ----------//

		void draw(const Camera &c, openGL::Shader &s);

        // cubes setters and getters

		/// return type at given position if exist else throw string error 
		/// catch(string const& str) {cerr << str << endl; }
		const uint8_t getType(const glm::uvec3 &pos);
		/// del value in our data array and octree coresponding id
		bool delAt(const glm::uvec3 &pos);
		bool setType(const glm::uvec3& pos, const uint8_t& type, const bool updateFaceMask = true);

        //---------- Getters & Setters ----------//

		glm::mat4 getModelMatrix() const;

        inline uint16_t capacity() const { return _cubes.capacity(); };
		unsigned int size() const;
        inline void setPosition(const glm::uvec3& pos) { _position = pos; }

		inline void updateAllFaceMask() { _cubes.updateAllFaceMask(); };

        //---------- Compatibility fonctions ----------//
		/*
        inline uint16_t coordToIndex(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {coordToIndex(glm::uvec3(x, y, z));}
        inline CubeVertex* getCube(const unsigned int &x, const unsigned int &y, const unsigned int &z) { return getCube(glm::uvec3(x, y, z));}
		inline void setCube(const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &type) {setCube(glm::uvec3(x, y, z), type); }
        inline bool validCoordinate(const unsigned int& x, const unsigned int& y, const unsigned int& z) const {validCoordinate(glm::uvec3(x, y, z));}
		*/
	};
}
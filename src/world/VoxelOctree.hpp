#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <algorithm> // for swap

#include "ListLinkedOctree.hpp"

namespace world {

    struct CubeVertex {
		glm::uvec3 pos;
		glm::vec3 color;
		uint8_t faceMask;

		CubeVertex() = default;
		CubeVertex(const glm::uvec3& p, const glm::vec3& c, const uint8_t& m) : pos(p), color(c), faceMask(m) {}
	};

	enum Direction { Up, Down, Left, Right, Front, Back };

	class  VoxelOctree : public ListLinkedOctree<CubeVertex> {
		private:
		//---------- private functions ----------//

		//privates methods used in mesh construction
		glm::uvec3 mortonIdToPos(const uint8_t &id) const;
		glm::uvec3 getPositionFromOctreeSubIndex(const std::vector<uint8_t> &OSubId) const;
		std::vector<std::pair<Direction, CubeVertex*>> getAdjacentsCubes(const glm::uvec3 &pos);

		// make there function from octree private
		using ListLinkedOctree::getValue;
		using ListLinkedOctree::delValue;
		using ListLinkedOctree::getPtrValue;

		public:

		VoxelOctree(const unsigned int &depth = 0); // root constructor 
		~VoxelOctree() = default;

		void updateAllFaceMask();

		//---------- Operators & functions ----------//
		/// return color at given position if exist else throw string error 
		/// catch(string const& str) {cerr << str << endl; }
		glm::vec3 getColor(const glm::uvec3 &pos);
		glm::vec3* getColorPtr(const glm::uvec3 &pos);
		/// del value in our data array and octree coresponding id
		bool delAt(const glm::uvec3 &pos, const bool updateFaceMask = true);
		bool setColor(const glm::uvec3 &pos, const glm::vec3 &color,  const bool updateFaceMask = true);
		// delete replaced method
		CubeVertex*& setValue(const glm::uvec3& pos, const CubeVertex& val) = delete;

	};

}// namespace world end

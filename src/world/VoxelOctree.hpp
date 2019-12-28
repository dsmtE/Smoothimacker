#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <algorithm> // for swap

#include "ListLinkedOctree.hpp"

namespace world {

    struct CubeVertex {
		glm::uvec3 pos;
		uint8_t type;
		uint8_t faceMask;

		CubeVertex() = default;
		CubeVertex(const glm::uvec3& p, const uint8_t& t, const uint8_t& m) : pos(p), type(t), faceMask(m) {}
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
		/// return type at given position if exist else throw string error 
		/// catch(string const& str) {cerr << str << endl; }
		uint8_t getType(const glm::uvec3 &pos);
		/// del value in our data array and octree coresponding id
		bool delAt(const glm::uvec3 &pos);
		bool setType(const glm::uvec3 &pos, const uint8_t &type,  const bool updateFaceMask = true);
		// delete replaced method
		CubeVertex*& setValue(const glm::uvec3& pos, const CubeVertex& val) = delete;
	};

}// namespace world end

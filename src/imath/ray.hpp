#include <glm/glm.hpp>
#include "../world/Chunk.hpp"
#include <array>

namespace imath {

	class Ray {

		glm::vec3 _origin;
		glm::vec3 _dir;
		// precalcuated value
		glm::vec3 _invDir;
		int _sign[3];

	public:
		Ray();
		Ray(const glm::vec3& o, const glm::vec3& d);
		/// if it colid with box on point p return t value ( o+t*d = p) else return 0
		bool colidWithBox(const std::array<glm::vec3, 2> &boxbounds, float &t);
		bool planeIntersect(const glm::vec3 p, const glm::vec3 n, float& t);
		
		inline glm::vec3 origin() { return _origin; };
		inline glm::vec3 dir() { return _dir; };
	};
	
}
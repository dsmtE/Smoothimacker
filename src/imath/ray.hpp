#include <glm/glm.hpp>

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
		unsigned int colidWithBox(const glm::vec3 &min, const glm::vec3 &max);
		
		inline glm::vec3 origin() { return _origin; };
		inline glm::vec3 dir() { return _dir; };
	};
}
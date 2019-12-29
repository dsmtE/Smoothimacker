#include "Ray.hpp"

#include <iostream>

using namespace imath;

Ray::Ray() : _origin(0, 0, 0), _dir(1, 0, 0) {
}

Ray::Ray(const glm::vec3& o, const glm::vec3& d) : _origin(o), _dir(d) {

    _invDir = glm::vec3(1, 1, 1) / _dir;
    _sign[0] = int(_invDir.x < 0);
    _sign[1] = int(_invDir.y < 0);
    _sign[2] = int(_invDir.z < 0);
}

bool Ray::colidWithBox(const std::array<glm::vec3, 2> &boxbounds, float &t) {

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (boxbounds[_sign[0]].x -_origin.x) * _invDir.x;
    tmax = (boxbounds[1 - _sign[0]].x -_origin.x) * _invDir.x;
    tymin = (boxbounds[_sign[1]].y -_origin.y) * _invDir.y;
    tymax = (boxbounds[1 - _sign[1]].y -_origin.y) * _invDir.y;
    if ((tmin > tymax) || (tymin > tmax)) {
        return false;
    }
    if (tymin > tmin) { // get the gretest min
        tmin = tymin;
    }
    if (tymax < tmax) { // get the smallest max
        tmax = tymax;
    }
    tzmin = (boxbounds[_sign[2]].z -_origin.z) * _invDir.z;
    tzmax = (boxbounds[1 - _sign[2]].z -_origin.z) * _invDir.z;
    if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
    }
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }
    t = tmin;
    return tmin > 0;
    // or with limit intersection to iterval [t0, t1] return ((tmin < t1) && (tmax > t0));
}

bool Ray::planeIntersect(const glm::vec3 p, const glm::vec3 n, float& t) {
    // vectors mus be normalized
    float denom =  glm::dot(n, _dir); // glm::dot(n, _dir);
    if (denom > 1e-6) {
        glm::vec3 numerator = p - _origin;
        t = glm::dot(numerator, n) / denom;
        return (t >= 0);
    }

    return false;
}

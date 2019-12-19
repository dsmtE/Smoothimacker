#pragma once

#include "../world/Octree.hpp"

namespace imath {

    class RadialBasisFunction {

    private:
        world::Octree _octree;
    public:

        RadialBasisFunction() = default;
        ~RadialBasisFunction() = default;

        unsigned int gaussian(float d, float alpha);
        unsigned int linear(float d, float alpha);
        unsigned int inverseQuadri(float d, float alpha);
        float dist(Eigen::Vector2f p1, Eigen::Vector2f p2);
        Eigen::VectorXd computeOmega(Eigen::Matrix3f X, Eigen::VectorXd u);
        Eigen::VectorXd phi(Eigen::Matrix3f X, Eigen::VectorXd u);

    };
}
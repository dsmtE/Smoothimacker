#pragma once

#include "../world/Chunk.hpp"
#include <Eigen/Dense>

//unsing namestapce insted of class
namespace imath::rbf {
        
    // radial fonctions
    float gaussian(float dist, float alpha);
    float linear(float dist, float alpha);
    float inverseQuadri(float dist, float alpha);
    //distance functions
    float dist(const Eigen::Vector2f &p1, const Eigen::Vector2f &p2);

    Eigen::VectorXf computeOmega(const std::vector<Eigen::Vector2f> &controlePts, const Eigen::VectorXf &controlePtsValues , float radialFunction(float, float));

    // change the name to make it more meaningful
    Eigen::VectorXf interpolate(const std::vector<Eigen::Vector2f> &evaluationPts, const std::vector<glm::vec3> &glmControlsPts, float radialFunction(float, float));

    void generateTerrain(world::Chunk &c, const std::vector<glm::vec3> &glmControlsPts, float rf(float, float)); // TODO
}
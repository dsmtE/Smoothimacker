#pragma once

#include "../world/Chunk.hpp"
#include <Eigen/Dense>
#include <functional>

//unsing namestapce insted of class
namespace imath::rbf {
        
    // radial fonctions
    float terrainLvlGaussian(float d, float alpha, float lvl);
    float terrainLvlQuadratic(float d, float alpha, float lvl);
    float gaussian(float dist, float alpha);
    float linear(float dist, float alpha);
    float inverseQuadri(float dist, float alpha);
    //distance functions
    float dist(const Eigen::Vector2f &p1, const Eigen::Vector2f &p2);

    Eigen::VectorXf computeOmega(const std::vector<Eigen::Vector2f> &controlePts, const Eigen::VectorXf &controlePtsValues , std::function< float(float) > radialFunction);

    // change the name to make it more meaningful
    Eigen::VectorXf interpolate(const std::vector<Eigen::Vector2f> &evaluationPts, const std::vector<glm::vec3> &glmControlsPts, std::function< float(float) > radialFunction);

    void generateTerrain(world::Chunk &c, const std::vector<glm::vec3> &glmControlsPts, std::function< float(float) > rf);


    glm::vec3 colorInterpolation(const float &coeff, const std::vector<std::pair<float, glm::vec3> > &thresholdAndColor);
}
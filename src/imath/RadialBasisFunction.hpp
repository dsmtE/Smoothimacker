#pragma once

#include "../world/Chunk.hpp"
#include <Eigen/Dense>
#include <functional>

//unsing namestapce insted of class
namespace imath::rbf {
        
    // radial fonctions
    float terrainLvlGaussian(const float &d, const float &alpha, const float &lvl);
    float terrainLvlQuadratic(const float &d, const float &alpha, const float &lvl);
    float terrainlvlSinus(const float &d, const float &alpha, const float &lvl, const float &freq);

    ///
    ///\brief return the distance between two points represented as vector
    ///
    ///\param p1 
    ///\param p2 
    ///\return float distance
    ///
    float dist(const Eigen::Vector2f &p1, const Eigen::Vector2f &p2);

    ///
    ///\brief compute omega value from controls points using Eigen (ColPivHouseholderQR)
    ///
    ///\param controlePts our controls points as 2D coordinates
    ///\param controlePtsValues our controles values (height) for each controle Pt
    ///\param radialFunction
    ///\return Eigen::VectorXf of omega values
    ///
    Eigen::VectorXf computeOmega(const std::vector<Eigen::Vector2f> &controlePts, const Eigen::VectorXf &controlePtsValues , const std::function< float(float) > &radialFunction);

    ///
    ///\brief interpolate
    ///
    ///\param evaluationPts our evaluation points as 2D coordinates where interplation will be compute
    ///\param glmControlsPts our control point as a vector of glm::vec3
    ///\param radialFunction 
    ///\return Eigen::VectorXf 
    ///
    Eigen::VectorXf interpolate(const std::vector<Eigen::Vector2f> &evaluationPts, const std::vector<glm::vec3> &glmControlsPts, const std::function< float(float) > &radialFunction);

    ///
    ///\brief function which takes as argument our chunk, and takes care, from the control points and the radial function, of emptying it and filling it with the new generated blocks.
    ///
    ///\param c 
    ///\param glmControlsPts 
    ///\param rf 
    ///
    void generateTerrain(world::Chunk &c, const std::vector<glm::vec3> &glmControlsPts, const std::function< float(float) > &rf);

    glm::vec3 colorInterpolation(const float &coeff, const std::vector<std::pair<float, glm::vec3> > &thresholdAndColor);
}
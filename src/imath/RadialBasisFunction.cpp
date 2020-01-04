#include <iostream>
#include<math.h>
#include <Eigen/Dense>
#include <algorithm> // for clamp

#include "RadialBasisFunction.hpp"

using namespace imath;

float rbf::terrainLvlGaussian(float d, float alpha, float lvl) { // lvl between [0, 1]
	return exp( log(1.0f - lvl) - alpha * d*d) + lvl;
}
float rbf::terrainLvlQuadratic(float d, float alpha, float lvl) {
	return 1.0f/(1.0f/(1.0f-lvl) + alpha * d*d) + lvl;
}

float rbf::gaussian(float d, float alpha) {
	return exp(alpha * d * d);
}

float rbf::linear(float d, float alpha) {
	return alpha * d;
}

float rbf::inverseQuadri(float d, float alpha) {
	return 1 / (1 + pow((alpha * d), 2));
}

float rbf::dist(const Eigen::Vector2f &p1, const Eigen::Vector2f &p2) {
	// return sqrt(pow((p1(0)- p2(0)), 2) + pow((p1(1) - p2(1)), 2));
	return (p1-p2).norm(); // Eigen does it better than us
}

Eigen::VectorXf rbf::computeOmega(const std::vector<Eigen::Vector2f> &controlePts, const Eigen::VectorXf &controlePtsValues , std::function< float(float) > radialFunction) {
	Eigen::MatrixXf A = Eigen::MatrixXf::Zero(controlePts.size(),controlePts.size());
	// build A with controlePts & radialFunction (cf cours)
	for (int j = 0; j < A.cols(); j++) { // size_t doesn't work ..
		for (int i = 0; i < A.rows(); i++) {
			A(i, j) = radialFunction(dist(controlePts[i], controlePts[j]));
		}
	}
	// solve system using Eigen
	Eigen::ColPivHouseholderQR<Eigen::MatrixXf> dec(A);
	return dec.solve(controlePtsValues);

	//Eigen::PartialPivLU<Eigen::MatrixXf> lu(A);
	//return lu.solve(controlePtsValues);
}

Eigen::VectorXf rbf::interpolate(const std::vector<Eigen::Vector2f> &evaluationPts, const std::vector<glm::vec3> &glmControlsPts, std::function< float(float) > radialFunction) {
	
	// glm to Eigen
	std::vector<Eigen::Vector2f> controlsPtsPos;
	controlsPtsPos.reserve(glmControlsPts.size());
	Eigen::VectorXf controlsPtsValues(glmControlsPts.size());

	for (size_t i = 0; i < glmControlsPts.size(); i++) {
		controlsPtsPos.push_back(Eigen::Vector2f(glmControlsPts[i].x, glmControlsPts[i].z));
		controlsPtsValues(i) = glmControlsPts[i].y;
	}
	
	Eigen::VectorXf w = rbf::computeOmega(controlsPtsPos, controlsPtsValues, radialFunction);
	// TODO return value at evaluation_pts with sum fonction
	Eigen::VectorXf interpolateValues = Eigen::VectorXf::Zero(evaluationPts.size());
	for (size_t i = 0; i < evaluationPts.size(); i++) {
		for (size_t j = 0; j < controlsPtsValues.size();j++) {
			interpolateValues(i) += w(j) * radialFunction(dist(controlsPtsPos[j], evaluationPts[i]));
		}
	}
	return interpolateValues;
}

void rbf::generateTerrain(world::Chunk &c, const std::vector<glm::vec3> &glmControlsPts, std::function< float(float) > rf) {
	// setup evaluation pts on 2D grid
	unsigned int chunckSize = c.size();
	std::vector<Eigen::Vector2f> evaluationPts;
	evaluationPts.reserve(chunckSize * chunckSize);
	for (size_t i = 0; i < chunckSize; i++) {
		for (size_t j = 0; j < chunckSize; j++) {
			evaluationPts.push_back(Eigen::Vector2f(i, j));
		}
	}
	// interpolate values
	Eigen::VectorXf interpolateValues  = interpolate(evaluationPts, glmControlsPts, rf);

	
	// const std::vector<std::pair<float, glm::vec3>> colorGradient = { 
	// 	{0.0f,  glm::vec3(0.0f, 0.502f, 0.267f)}, //blue
	// 	{0.62f, glm::vec3(0.0f, 0.059f, 0.945f)}, //green
	// 	{1.0f,  glm::vec3(0.059f, 0.937f, 0.624f)}  // white
	// };
	const std::vector<std::pair<float, glm::vec3>> colorGradient = { 
		{0.0f, glm::vec3(8,4,65)/255.0f},
		{0.01f, glm::vec3(9,9,121)/255.0f},
		{0.05f, glm::vec3(0,212,255)/255.0f},
		{0.06f, glm::vec3(242,229,171)/255.0f},
		{0.07f, glm::vec3(92,59,17)/255.0f},
		{0.10f, glm::vec3(156,88,5)/255.0f},
		{0.14f, glm::vec3(0,255,20)/255.0f},
		{0.72f, glm::vec3(16,187,27)/255.0f},
		{0.79f, glm::vec3(98,78,51)/255.0f},
		{0.81f, glm::vec3(94,69,39)/255.0f},
		{0.85f, glm::vec3(252,231,205)/255.0f},
		{1.0f, glm::vec3(254,249,243)/255.0f}
	};

	// reset
	c.reset();
	// fill chunk
	for (int i = 0; i < interpolateValues.size(); i++) { // for each interpolateValue
		for (unsigned int h = 0; h < std::clamp(unsigned int (floor(interpolateValues[i])), unsigned int(0), c.size()-1); h++) {
			
			// c.setColor(glm::uvec3(unsigned int(evaluationPts[i](0)), unsigned int(h), unsigned int(evaluationPts[i](1))), glm::vec3(0.0f, 1.0f, 0.0f), false);
			c.setColor(glm::uvec3(unsigned int(evaluationPts[i](0)), h, unsigned int(evaluationPts[i](1))), colorInterpolation(float(h)/float(c.size()-1), colorGradient), false);
		}
	}
	c.updateAllFaceMask();
}

    glm::vec3 rbf::colorInterpolation(const float &coeff, const std::vector<std::pair<float, glm::vec3> > &thresholdAndColor) {
		assert(thresholdAndColor.front().first == 0.0f && thresholdAndColor.back().first == 1.0f && coeff >= 0.0f && coeff <= 1.0f );
        size_t id = 1;
        // find the index where our coefficient is included in the table of thresholds&colors
        while (coeff > thresholdAndColor[id].first && id < thresholdAndColor.size()-1) {
            id++;
        }
        return glm::mix(thresholdAndColor[id-1].second, thresholdAndColor[id].second,(coeff-thresholdAndColor[id-1].first)/(thresholdAndColor[id].first-thresholdAndColor[id-1].first) );
    }
#include <iostream>
#include<math.h>
#include <Eigen/Dense>
#include <algorithm> // for clamp

#include "RadialBasisFunction.hpp"

using namespace imath;

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

Eigen::VectorXf rbf::computeOmega(const std::vector<Eigen::Vector2f> &controlePts, const Eigen::VectorXf &controlePtsValues , float radialFunction(float, float)) {
	Eigen::MatrixXf A = Eigen::MatrixXf::Zero(controlePts.size(),controlePts.size());
	// build A with controlePts & radialFunction (cf cours)
	for (int j = 0; j < A.cols(); j++) { // size_t doesn't work ..
		for (int i = 0; i < A.rows(); i++) {
			A(i, j) = radialFunction(dist(controlePts[i], controlePts[j]), -0.2f);
		}
	}
	// solve system using Eigen
	Eigen::ColPivHouseholderQR<Eigen::MatrixXf> dec(A);
	return dec.solve(controlePtsValues);

	//Eigen::PartialPivLU<Eigen::MatrixXf> lu(A);
	//return lu.solve(controlePtsValues);
}

Eigen::VectorXf rbf::interpolate(const std::vector<Eigen::Vector2f> &evaluationPts, const std::vector<glm::vec3> &glmControlsPts, float radialFunction(float, float)) {
	
	// glm to Eigen
	std::vector<Eigen::Vector2f> controlsPtsPos;
	controlsPtsPos.reserve(glmControlsPts.size());
	Eigen::VectorXf controlsPtsValues(glmControlsPts.size());

	for (size_t i = 0; i < glmControlsPts.size(); i++) {
		controlsPtsPos.push_back(Eigen::Vector2f(glmControlsPts[i].x, glmControlsPts[i].y));
		controlsPtsValues(i) = glmControlsPts[i].z;
	}
	
	Eigen::VectorXf w = rbf::computeOmega(controlsPtsPos, controlsPtsValues, radialFunction);
	// TODO return value at evaluation_pts with sum fonction
	Eigen::VectorXf interpolateValues = Eigen::VectorXf::Zero(evaluationPts.size());
	for (size_t i = 0; i < evaluationPts.size(); i++) {
		for (size_t j = 0; j < controlsPtsValues.size();j++) {
			interpolateValues(i) += w(j) * radialFunction(dist(controlsPtsPos[j], evaluationPts[i]), -0.2f);
		}
	}
	return interpolateValues;
}

void rbf::generateTerrain(world::Chunk &c, const std::vector<glm::vec3> &glmControlsPts, float rf(float, float)) {
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
	Eigen::VectorXf interpolateValues = interpolate(evaluationPts, glmControlsPts, rf);

	// reset
	c.reset();
	// fill chunk
	for (int i = 0; i < interpolateValues.size(); i++) { // for each interpolateValue
		for (size_t h = 0; h < std::clamp(unsigned int (floor(interpolateValues[i])), unsigned int(0), c.size()-1); h++) {
			c.setColor(glm::uvec3(unsigned int(evaluationPts[i](0)), unsigned int(h), unsigned int(evaluationPts[i](1))), glm::vec3(0.0f, 1.0f, 0.0f), false);
		}
	}
	c.updateAllFaceMask();
}
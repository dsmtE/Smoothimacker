#include <iostream>
#include<math.h>
#include <Eigen/Dense>

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
	// return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
	return (p1-p2).norm(); // Eigen does it better than us
}

Eigen::VectorXf rbf::computeOmega(const std::vector<Eigen::Vector2f> &controle_pts, const Eigen::VectorXf &controle_pts_Values , float radialFunction(float, float)) {
	// TODO build A with controle_pts (cf cours)
	Eigen::MatrixXf A;
	Eigen::ColPivHouseholderQR<Eigen::MatrixXf> dec(A);
	return dec.solve(controle_pts_Values);
}

Eigen::VectorXf  rbf::interpolate(const std::vector<Eigen::Vector2f> &evaluation_pts, const std::vector<Eigen::Vector2f> &controle_pts, const Eigen::VectorXf &controle_pts_Values, float radialFunction(float, float)) {
	Eigen::VectorXf w = rbf::computeOmega(controle_pts, controle_pts_Values , radialFunction);
	// TODO return value at evaluation_pts with sum fonction
	return w;
}


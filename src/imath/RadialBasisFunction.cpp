#include <iostream>
#include<math.h>
#include <Eigen/Dense>

#include <RadialBasisFunction.hpp>

using namespace imath;

unsigned int RadialBasisFunction::gaussian(float d, float alpha) {
	return exp(alpha * d * d);
}

unsigned int RadialBasisFunction::linear(float d, float alpha) {
	return alpha * d;
}

unsigned int RadialBasisFunction::inverseQuadri(float d, float alpha) {
	return 1 / (1 + pow((alpha * d), 2));
}

float RadialBasisFunction::dist(Eigen::Vector2f p1, Eigen::Vector2f p2) {
	return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

Eigen::VectorXd RadialBasisFunction::computeOmega(Eigen::Matrix3f X, Eigen::VectorXd u) {
	ColPivHouseholderQR<Matrix3f> dec(X);
	return dec.solve(u);
}

Eigen::VectorXd RadialBasisFunction::phi(Eigen::Matrix3f X, Eigen::VectorXd u) {
	Eigen::VectorXd wk = computeOmega(X, u);

	// return sum phi
	// return [[ np.sum([wk_ * rbf(dist2D(p, pk_)) for pk_, wk_ in temp]) for p in line]for line in pts]
}

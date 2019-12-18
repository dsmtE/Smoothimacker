#include <iostream>

#include "ExtrudeStrategy.hpp"

using namespace gui;

ExtrudeStrategy::~ExtrudeStrategy() {

}

void ExtrudeStrategy::action() {
	std::cout << "extrude" << std::endl;
}

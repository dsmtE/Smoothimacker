#include <iostream>

#include "PaintStrategy.hpp"

using namespace gui;

PaintStrategy::~PaintStrategy() {

}

void PaintStrategy::action() {
	std::cout << "paint" << std::endl;
}


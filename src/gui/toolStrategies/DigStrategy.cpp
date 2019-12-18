#include <iostream>

#include "DigStrategy.hpp"

using namespace gui;

DigStrategy::~DigStrategy() {

}

void DigStrategy::action() {
	std::cout << "dig" << std::endl;
}

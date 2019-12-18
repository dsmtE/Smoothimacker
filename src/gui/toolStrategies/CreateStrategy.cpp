#include <iostream>

#include "CreateStrategy.hpp"

using namespace gui;

CreateStrategy::~CreateStrategy() {

}

void CreateStrategy::action() {
	std::cout << "create" << std::endl;
}

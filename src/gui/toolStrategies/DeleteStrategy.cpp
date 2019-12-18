#include <iostream>

#include "DeleteStrategy.hpp"

using namespace gui;

DeleteStrategy::~DeleteStrategy() {

}

void DeleteStrategy::action() {
	std::cout << "delete" << std::endl;
}

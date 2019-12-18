#include <iostream>

#include "Tool.hpp"

#include "toolStrategies/IToolStrategy.hpp"

#include "toolStrategies/CreateStrategy.hpp"
#include "toolStrategies/DeleteStrategy.hpp"
#include "toolStrategies/ExtrudeStrategy.hpp"
#include "toolStrategies/DigStrategy.hpp"
#include "toolStrategies/PaintStrategy.hpp"

using namespace gui;

Tool::Tool() : _strategy(nullptr) {}

Tool::~Tool() {
	if (_strategy != nullptr) {
		delete _strategy;
	}
}

void Tool::handleSDLEvents(SDL_Event sdlEvent) {
	if (sdlEvent.type == SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.sym == SDLK_a) {
			if (_strategy != nullptr) {
				_strategy->action();
			}
			else {
				std::cerr << "Error : Strategy doesn't exist" << std::endl;
			}	
		}
	}
}

void Tool::setStrategy(StrategyType type) {
	if (_strategy != nullptr) {
		delete _strategy;
	}
	if (type == Create) {
		_strategy = new CreateStrategy();
	}
	if (type == Delete) {
		_strategy = new DeleteStrategy();
	}
	if (type == Extrude) {
		_strategy = new ExtrudeStrategy();
	}
	if (type == Dig) {
		_strategy = new DigStrategy();
	}
	if (type == Paint) {
		_strategy = new PaintStrategy();
	}
}
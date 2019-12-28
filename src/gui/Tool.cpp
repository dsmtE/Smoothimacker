#include <iostream>

#include "Tool.hpp"


using namespace gui;

Tool::Tool() : _action([] {std::cout << "action not assigned yet" << std::endl; }) {}


void Tool::handleSDLEvents(SDL_Event sdlEvent) {
	if (sdlEvent.type == SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.sym == SDLK_a) {
			_action();
		}
	}
}

void Tool::setAction(std::function<void()> action) {
	_action = action;
}
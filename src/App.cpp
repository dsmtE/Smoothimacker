#include "App.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <glm/gtx/norm.hpp>

#include <iostream>

App::App(int width, int height, const char* title) : 
	IApp(width, height, title),
	_cam(glm::vec3(0, 0, 5.f)),
	_cursorShader("assets/shaders/cursor.vert", "assets/shaders/cursor.frag"),
	_chunkShader("assets/shaders/chunk.vert", "assets/shaders/chunk.frag", "assets/shaders/chunk.geom"),
	_relativeMouse(SDL_FALSE) ,
	_menu(_cursor.getPointerPos()), 
	_chunk(16) {
	SDL_SetRelativeMouseMode(SDL_FALSE);
	_cursor.setCameraReference(_cam); // set cam as reference for cursor mouvement with keyboard
	//fill chunk 
	for (unsigned int i = 0; i < _chunk.size(); i++) {
		for (unsigned int j = 0; j < _chunk.size(); j++) {
			for (unsigned int k = 0; k < _chunk.size(); k++) {
				_chunk.setCube(i, j, k, j); 
			}
		}
	}
}

void App::handleEvents() {
	IApp::handleEvents(); // delegate SDL events to App::handleSDLEvents(SDL_Event sdlEvent)
	_cam.handleEvents(_deltaTime);
}

void App::handleSDLEvents(SDL_Event sdlEvent) {
	_cursor.handleEvent(sdlEvent);
	_menu.handleEvent(sdlEvent);

	_cam.handleRotationEvents(sdlEvent, _relativeMouse);
	
	// application event
	switch (sdlEvent.type) {
		case SDL_QUIT:
			exit();
			break;

		case SDL_KEYUP:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				exit();
			}
			break;

		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_r) {
				if (_relativeMouse) {
					_relativeMouse = SDL_FALSE;
				}else {
					_relativeMouse = SDL_TRUE; 
				}
				SDL_SetRelativeMouseMode(_relativeMouse);
			}
			else {
				//printf("Keycode: %s (%d) Scancode: %s (%d) \n",
				//SDL_GetKeyName(sdlEvent.key.keysym.sym), sdlEvent.key.keysym.sym,
				//SDL_GetScancodeName(sdlEvent.key.keysym.scancode), sdlEvent.key.keysym.scancode);
			}
			break;

		default:
			// std::cout << sdlEvent.type << std::endl;
			break;

	}
}

void App::loop() {
	int w = windowSize().x;
	int h = windowSize().y;
	while (isRunning()) {
		beginFrame();
		_menu.drawMenu();
		_chunk.draw(_cam, w, h, _chunkShader);

		glClear(GL_DEPTH_BUFFER_BIT); // for cursor overlay
		_cursor.draw(_cam, w, h, _cursorShader);

		endFrame();
	}
}

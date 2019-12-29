#include "App.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <glm/gtx/norm.hpp>

#include <iostream>

#include <chrono>

App::App(int width, int height, const char* title) : 
	IApp(width, height, title),
	_cam(width, height, glm::vec3(0, 0, 5.f)),
	_cursorShader("assets/shaders/cursor.vert", "assets/shaders/cursor.frag"),
	_chunkShader("assets/shaders/chunk.vert", "assets/shaders/chunk.frag", "assets/shaders/chunk.geom"),
	_chunk(5),
	_settings(_cursor.getPointerPos(), _cam.getCameraSpeedPtr(), &_window),
	_menu(&_chunk, &_settings) {

	SDL_SetRelativeMouseMode(SDL_FALSE);
	_cursor.setCameraReference(_cam); // set cam as reference for cursor mouvement with keyboard

	auto t1 = std::chrono::high_resolution_clock::now();

	//fill chunk
	for (unsigned int i = 0; i < _chunk.size(); i++) {
		for (unsigned int j = 0; j < _chunk.size(); j++) {
			for (unsigned int k = 0; k < _chunk.size(); k++) {
				_chunk.setType(glm::uvec3(i, k, j), i); 
			}
		}
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff1 = t2 - t1;
	std::cout << "Elapsed time for add cube: " << diff1.count() << " s\n";

	_chunk.updateAllFaceMask();

	auto t3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff2 = t3 - t2;
	std::cout << "Elapsed time for mesh calc: " << diff2.count() << " s\n";

}

void App::handleEvents() {
	IApp::handleEvents(); // delegate SDL events to App::handleSDLEvents(SDL_Event sdlEvent)
	_cam.handleEvents(_deltaTime);
}

void App::handleSDLEvents(SDL_Event sdlEvent) {
	_cursor.handleEvent(sdlEvent);
	_menu.handleEvent(sdlEvent);

	_cam.handleRotationEvents(sdlEvent, _settings.relativeMouse());
	
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
				if (_settings.relativeMouse()) {
					_settings.relativeMouse() = SDL_FALSE;
				}else {
					_settings.relativeMouse() = SDL_TRUE;
				}
				SDL_SetRelativeMouseMode(_settings.relativeMouse());
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
	while (isRunning()) {
		beginFrame();
		_menu.drawMenu();
		_chunk.draw(_cam, _chunkShader);

		glClear(GL_DEPTH_BUFFER_BIT); // for cursor overlay
		_cursor.draw(_cam, _cursorShader);

		endFrame();
	}
}
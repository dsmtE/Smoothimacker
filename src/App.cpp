#include "App.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <glm/gtx/norm.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

#include <chrono>


App::App(int width, int height, const char* title) :
	IApp(width, height, title),
	_cam(width, height, glm::vec3(0.0f, 10.0f, 0.0f)),
	_cursorShader("assets/shaders/cursor.vert", "assets/shaders/cursor.frag"),
	_chunkShader("assets/shaders/chunk.vert", "assets/shaders/chunk.frag", "assets/shaders/chunk.geom"),
	_controlPtsShader("assets/shaders/controlPts.vert", "assets/shaders/controlPts.frag"),
	_gridShader("assets/shaders/grid.vert", "assets/shaders/grid.frag"),
	_pointsLightsShader("assets/shaders/pointsLights.vert", "assets/shaders/pointsLights.frag"),
	_chunk(6),
	_controlPts(),
	_cursor(0, _chunk.size()),
	_grid(_chunk.size()),
	_pointLights(), 
	_settings(_cursor.getPointerPos(), _cam.getCameraSpeedPtr(), &_chunk, &_window, &_controlPts, &_pointLights),
	_menu(&_chunk, &_settings) {

	SDL_SetRelativeMouseMode(SDL_FALSE);
	_cursor.setCameraReference(_cam); // set cam as reference for cursor mouvement with keyboard


	auto t1 = std::chrono::high_resolution_clock::now();

	//fill chunk
	for (unsigned int i = 0; i < _chunk.size(); i++) {
		for (unsigned int j = 0; j < _chunk.size(); j++) {
			for (unsigned int k = 0; k < 3; k++) {
				_chunk.setColor(glm::uvec3(i, k, j), glm::vec3(0.435f, 0.569f, 0.133f));
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

	_cam.handleRotationEvents(sdlEvent, _settings._relativeMouse);
	
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
				if (_settings._relativeMouse) {
					_settings._relativeMouse = SDL_FALSE;
				}else {
					_settings._relativeMouse = SDL_TRUE;
				}
				SDL_SetRelativeMouseMode(_settings._relativeMouse);
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

		if(_settings._animSun) {
			_settings._sunDir = glm::rotate(_settings._sunDir, 0.0008f, glm::vec3(1.0f, 0.0f, -0.6f));
			if ( _settings._sunDir.y > -0.1f) {// accelerate during night (when sundir is under our scene)
				_settings._sunDir = glm::rotate(_settings._sunDir, 0.005f, glm::vec3(1.0f, 0.0f, -0.6f));
			}
		}

		_menu.drawMenu();
		_grid.draw(_cam, _gridShader);
		_pointLights.draw(_cam, _pointsLightsShader);
		_chunk.draw(_cam, _chunkShader, _settings._sunDir, _settings._sunColor, _settings._pointLights->getLights(), _settings._dayMode); 

		glClear(GL_DEPTH_BUFFER_BIT); // for cursor overlay
		_cursor.draw(_cam, _cursorShader);
		_controlPts.draw(_cam, _controlPtsShader);

		endFrame();
	}
}
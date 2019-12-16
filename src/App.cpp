#include "App.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <iostream>

App::App(int width, int height, const char* title, const world::Camera& cam)
	: IApp(width, height, title), _cam(cam), _deltaMouse(0, 0), _deltaWheel(0, 0), _mouseMode(SDL_FALSE) {
	SDL_SetRelativeMouseMode(_mouseMode);
}

void App::handleEvents() {
	IApp::handleEvents();
	_cam.handleEvent(_deltaTime, _deltaMouse, _deltaWheel);
	// reset infos (if mouse doesn't move)
	_deltaMouse = glm::ivec2(0, 0);
	_deltaWheel = glm::ivec2(0, 0);
}

void App::handleSDLEvents(SDL_Event sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_QUIT:
		exit();
		break;

	case SDL_MOUSEWHEEL:
		_deltaWheel = glm::ivec2(sdlEvent.wheel.x, sdlEvent.wheel.y);
		break;

	case SDL_MOUSEMOTION:
		_deltaMouse = glme::ivec2(sdlEvent.motion.wrel, sdlEvent.motion.yrel);
		break;

	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.sym) {

		default:
			break;
		}
		if (sdlEvent.key.keysym.sym == SDLK_m) {//  M swith mouse lock
			if (_mouseMode) {
				_mouseMode = SDL_FALSE;
			}
			else { _mouseMode = SDL_TRUE; }
			SDL_SetRelativeMouseMode(_mouseMode);
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

void App::renderMenu() {
	ImGui::Begin("Main debug window");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::CollapsingHeader("Help")) {
		ImGui::Text("Camera controls :");
		ImGui::BulletText("Orbit - Left mouse button / Middle mouse button");
		ImGui::BulletText("Pan - Right mouse button");
		ImGui::BulletText("Zoom - Mousewheel");
		ImGui::BulletText("Reset - Left mouse double click");
	}

	ImGui::Spacing();

	ImGui::Text("Exemples:");
	if (ImGui::CollapsingHeader("Basic")) {
		if (ImGui::Button("test Button")) { std::cout << "test boutton " << std::endl; }
	}

	ImGui::End();
}

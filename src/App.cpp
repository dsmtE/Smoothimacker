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
	_relativeMouse(SDL_FALSE) ,
	_menu(_cursor.getPointerPos() ) {
	SDL_SetRelativeMouseMode(SDL_FALSE);
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
		_cursorShader.bind();
		_cursorShader.setMat4("view", _cam.getViewMatrix());
		_cursorShader.setMat4("projection", _cam.getProjectionMatrix(w, h));
		_cursorShader.setMat4("model", _cursor.getModelMatrix());

		_cursor.draw(_cursorShader);

		_menu.drawMenu();
		
		_cursorShader.unBind();

		endFrame();
	}
}

/*
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
*/

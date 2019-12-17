#include "IApp.hpp"

#include <glad/glad.h>
#include <SDL.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include "openGL/glException.hpp"

IApp::IApp(int width, int height, const char* title) :
	_running(true), _window(width, height, title), _deltaTime(0), _lastTime(SDL_GetTicks()) {

}

void IApp::beginFrame() {
	GLCall(glClearColor(0.102, 0.117f, 0.141f, 1.0f)); // set background color
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // clear
	_window.updateImgui(); 
}

void IApp::handleEvents() {
	SDL_Event e;
	while (_window.handleSDLEvents(e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		handleSDLEvents(e);
	}
}

void IApp::endFrame() {
	_window.renderImgui();
	_window.swapBuffers();

	handleEvents();
	unsigned int currentTime = SDL_GetTicks();
	_deltaTime = currentTime - _lastTime;
	_lastTime = currentTime;
}
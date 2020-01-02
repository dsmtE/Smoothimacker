#include "SDLWinWrapper.hpp"

#include <iostream>
#include <imgui/imgui.h>
#include<imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <glad/glad.h>

using namespace openGL;

void SDLWinWrapper::initSDL() {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "[SDL2] Unable to initialize SDL:" << SDL_GetError() << std::endl;
    }
    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetSwapInterval(1); // V-Sync
}

void SDLWinWrapper::initImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(_window, _glContext);
    ImGui_ImplOpenGL3_Init("#version 440 core");
    ImGui::StyleColorsDark();
}

void SDLWinWrapper::swapBuffers() {
    SDL_GL_SwapWindow(_window); // Swap the screen buffers
}

void SDLWinWrapper::updateImgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

}

void SDLWinWrapper::renderImgui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int SDLWinWrapper::handleSDLEvents(SDL_Event& e) {
    return SDL_PollEvent(&e);
}

glm::ivec2 SDLWinWrapper::getMouseCoord() const {
	glm::ivec2 coord;
	SDL_GetMouseState(&(coord.x), &(coord.y));
	return coord;
}

SDLWinWrapper::SDLWinWrapper(int width, int height, const char* title) : _running(true) {
        
    initSDL();

    // Create window
    _window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // or SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_OPENGL //| SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    // Create OpenGL context
    if (_window == nullptr) {
        std::cerr << "[SDL2] Window is null: " << SDL_GetError() << std::endl;
    }

    _glContext = SDL_GL_CreateContext(_window);
    if (_glContext == nullptr) {
        std::cerr << "[SDL2] OpenGL context is null: " <<  SDL_GetError() << std::endl;
    }

	// Load OpenGL functions glad SDL
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cerr << "[Glad] Glad not init" << std::endl;
	}

    initImgui();
    //others openGL settings
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

SDLWinWrapper::~SDLWinWrapper() {
    // imgui shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    //sdl shutdown
    SDL_DestroyWindow(_window);
    SDL_Quit();
}


glm::ivec2 SDLWinWrapper::windowSize() const {
    glm::ivec2 size;
    SDL_GetWindowSize(_window, &(size.x), &(size.y));
    return size;
}
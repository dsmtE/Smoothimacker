#include <iostream>
#include <string>

// SDL
#include <SDL.h>

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

// GLAD
#include <glad/glad.h>

// GLM & glm transform
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App.hpp"

int main(int argc, char *argv[]) {
    
  std::cout << "---------- Smoothimacker ----------" << std::endl;
  App app(1280, 720, "Smoothimacker");
  app.loop();

  return 0;
}